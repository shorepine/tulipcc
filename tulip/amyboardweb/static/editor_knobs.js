function init_knobs(knobConfigs, gridId, onChange) {
  const targetId = gridId || "knob-grid";
  const grid = document.getElementById(targetId);
  if (!grid || !Array.isArray(knobConfigs)) {
    return;
  }

  grid.innerHTML = "";

  function clamp(value, min, max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
  }

  function parseNumber(value, fallback) {
    const parsed = Number(value);
    return Number.isFinite(parsed) ? parsed : fallback;
  }

  function isPartialNumber(raw) {
    return raw === "" || raw === "-" || raw === "." || raw === "-.";
  }

  function getStep(min, max) {
    const span = Math.abs(max - min);
    if (!Number.isFinite(span) || span === 0) {
      return 0.01;
    }
    return Math.max(span / 1000, 0.0001);
  }

  function formatValue(value, decimals) {
    return value.toFixed(decimals);
  }

  function notifyKnobChange(index, value, config, options) {
    if (typeof onChange === "function") {
      onChange(index, value);
    } else if (typeof window.onKnobChange === "function") {
      window.onKnobChange(index, value);
    }
    const notifyAmy = !options || options.notifyAmy !== false;
    if (notifyAmy && config && typeof config.onChange === "function") {
      config.onChange(value);
    }
  }

  function renderKnob(config, index, targetGrid) {
    const colClass = targetId === "knob-grid" ? "col-6 col-md-3 text-center knob-col" : "col-6 text-center";
    if (config.knob_type === "selection") {
      const options = Array.isArray(config.options) ? config.options : [];
      const defaultIndex = parseNumber(config.default_value, 0);
      const clampedIndex = clamp(defaultIndex, 0, Math.max(options.length - 1, 0));
      const displayName = config.display_name || `Select ${index + 1}`;

      const col = document.createElement("div");
      col.className = colClass;

      const label = document.createElement("div");
      label.className = "small mb-2";
      label.textContent = displayName;

      const selectWrap = document.createElement("div");
      selectWrap.className = "amy-select";

      const select = document.createElement("select");
      select.size = Math.min(Math.max(options.length, 3), 6);
      select.setAttribute("data-index", String(index));

      options.forEach(function(option, optIndex) {
        const opt = document.createElement("option");
        opt.value = String(optIndex);
        opt.textContent = String(option);
        select.appendChild(opt);
      });

      select.value = String(clampedIndex);

      select.addEventListener("change", function() {
        const idx = parseNumber(select.value, 0);
        notifyKnobChange(index, idx, config, { notifyAmy: true });
      });

      selectWrap.appendChild(select);
      col.appendChild(label);
      col.appendChild(selectWrap);
      targetGrid.appendChild(col);
      return;
    }

    const isLogKnob = config.knob_type === "log";
    const logRange = parseNumber(config.range, 1);
    const logOffset = parseNumber(config.offset, 1);
    const min = isLogKnob ? 0 : parseNumber(config.min_value, 0);
    const max = isLogKnob ? 1 : parseNumber(config.max_value, 1);
    const defaultValue = parseNumber(config.default_value, min);
    const displayName = config.display_name || `Knob ${index + 1}`;
    const step = getStep(min, max);
    const displayDecimals = 3;

    const col = document.createElement("div");
    col.className = colClass;

    const label = document.createElement("div");
    label.className = "small mb-2";
    label.textContent = displayName;

    const knob = document.createElement("div");
    knob.className = "amy-knob";
    knob.setAttribute("role", "slider");
    knob.setAttribute("tabindex", "0");

    const face = document.createElement("div");
    face.className = "amy-knob-face";

    const indicator = document.createElement("div");
    indicator.className = "amy-knob-indicator";
    if (isLogKnob) {
      indicator.classList.add("amy-knob-indicator-log");
    }

    face.appendChild(indicator);
    knob.appendChild(face);

    const number = document.createElement("input");
    number.className = "form-control form-control-sm knob-value mt-2 text-center";
    number.type = "number";
    function logOutputFor(value) {
      return logOffset * Math.exp(logRange * value) - logOffset;
    }

    const displayRange = (function() {
      if (!isLogKnob) {
        return { min: min, max: max };
      }
      const outMin = logOutputFor(min);
      const outMax = logOutputFor(max);
      if (!Number.isFinite(outMin) || !Number.isFinite(outMax)) {
        return { min: 0, max: 1 };
      }
      return { min: Math.min(outMin, outMax), max: Math.max(outMin, outMax) };
    })();

    const displayStep = getStep(displayRange.min, displayRange.max);

    knob.setAttribute("aria-valuemin", String(displayRange.min));
    knob.setAttribute("aria-valuemax", String(displayRange.max));
    number.min = String(displayRange.min);
    number.max = String(displayRange.max);
    number.step = String(displayStep);

    col.appendChild(label);
    col.appendChild(knob);
    col.appendChild(number);
    targetGrid.appendChild(col);

    const angleStart = 210;
    const angleEnd = 510;
    const angleSpan = angleEnd - angleStart;

    const state = {
      value: clamp(defaultValue, min, max),
      dragging: false,
      startX: 0,
      startY: 0,
      startValue: 0,
      dragStep: Math.abs(max - min) / 150
    };

    function valueToAngle(value) {
      if (max === min) return angleStart;
      const normalized = (value - min) / (max - min);
      return angleStart + clamp(normalized, 0, 1) * angleSpan;
    }

    function outputToValue(output) {
      if (!Number.isFinite(output)) {
        return state.value;
      }
      if (logRange === 0 || logOffset === 0) {
        return output === 0 ? min : state.value;
      }
      const ratio = (output + logOffset) / logOffset;
      if (!Number.isFinite(ratio) || ratio <= 0) {
        return state.value;
      }
      const raw = Math.log(ratio) / logRange;
      if (!Number.isFinite(raw)) {
        return state.value;
      }
      return clamp(raw, min, max);
    }

    function setValue(nextValue, commit, notifyAmy) {
      const clamped = clamp(nextValue, min, max);
      state.value = clamped;
      const angle = valueToAngle(clamped);
      face.style.setProperty("--angle", `${angle}deg`);
      const outputValue = isLogKnob ? logOutputFor(clamped) : clamped;
      knob.setAttribute("aria-valuenow", formatValue(outputValue, displayDecimals));
      if (commit) {
        number.value = formatValue(outputValue, displayDecimals);
      }
      notifyKnobChange(index, outputValue, config, { notifyAmy: notifyAmy });
    }

    setValue(state.value, true, false);

    knob.addEventListener("pointerdown", function(event) {
      event.preventDefault();
      knob.setPointerCapture(event.pointerId);
      state.dragging = true;
      state.startX = event.clientX;
      state.startY = event.clientY;
      state.startValue = state.value;
    });

    knob.addEventListener("pointermove", function(event) {
      if (!state.dragging) return;
      const dx = event.clientX - state.startX;
      const dy = event.clientY - state.startY;
      const delta = (-dy + dx) * state.dragStep;
      setValue(state.startValue + delta, true, true);
    });

    knob.addEventListener("pointerup", function(event) {
      if (!state.dragging) return;
      state.dragging = false;
      knob.releasePointerCapture(event.pointerId);
    });

    knob.addEventListener("pointercancel", function(event) {
      if (!state.dragging) return;
      state.dragging = false;
      knob.releasePointerCapture(event.pointerId);
    });

    number.addEventListener("input", function() {
      if (isPartialNumber(number.value)) {
        return;
      }
    });

    number.addEventListener("keydown", function(event) {
      if (event.key === "Enter") {
        const parsed = parseNumber(number.value, isLogKnob ? logOutputFor(state.value) : state.value);
        const rawValue = isLogKnob ? outputToValue(parsed) : parsed;
        setValue(rawValue, true, true);
      }
    });

    number.addEventListener("change", function() {
      const parsed = parseNumber(number.value, isLogKnob ? logOutputFor(state.value) : state.value);
      const rawValue = isLogKnob ? outputToValue(parsed) : parsed;
      setValue(rawValue, true, true);
    });

    number.addEventListener("blur", function() {
      const parsed = parseNumber(number.value, isLogKnob ? logOutputFor(state.value) : state.value);
      const rawValue = isLogKnob ? outputToValue(parsed) : parsed;
      setValue(rawValue, true, true);
    });

    knob.addEventListener("keydown", function(event) {
      if (event.key === "ArrowUp" || event.key === "ArrowRight") {
        event.preventDefault();
        setValue(state.value + step, true, true);
      }
      if (event.key === "ArrowDown" || event.key === "ArrowLeft") {
        event.preventDefault();
        setValue(state.value - step, true, true);
      }
    });
  }

  const sections = [];
  let currentSection = null;

  knobConfigs.forEach(function(config, index) {
    const sectionName = typeof config.section === "string" ? config.section : "";
    if (!currentSection || currentSection.name !== sectionName) {
      currentSection = { name: sectionName, items: [] };
      sections.push(currentSection);
    }
    currentSection.items.push({ config: config, index: index });
  });

  sections.forEach(function(section) {
    const sectionWrap = document.createElement("div");
    const sectionClass = targetId === "knob-grid" ? "col-12 knob-section knob-section-main" : "col-12 knob-section";
    sectionWrap.className = sectionClass;
    sectionWrap.style.setProperty("--knob-count", String(section.items.length));

    const header = document.createElement("div");
    header.className = "knob-section-header";
    header.textContent = section.name || "";

    const sectionGrid = document.createElement("div");
    sectionGrid.className = targetId === "knob-grid" ? "row g-4 knob-section-grid" : "row g-3 knob-section-grid";

    sectionWrap.appendChild(header);
    sectionWrap.appendChild(sectionGrid);
    grid.appendChild(sectionWrap);

    section.items.forEach(function(item) {
      renderKnob(item.config, item.index, sectionGrid);
    });
  });
}

function set_amy_knob_value(knobs, name, value) {
  if (!Array.isArray(knobs) || !Number.isFinite(value)) {
    return false;
  }
  const knob = knobs.find((entry) => entry.display_name === name);
  if (!knob) {
    return false;
  }
  if (knob.knob_type === "log") {
    const offset = Number(knob.offset);
    const range = Number(knob.range);
    if (!Number.isFinite(offset) || !Number.isFinite(range) || offset === 0 || range === 0) {
      return false;
    }
    const ratio = (value + offset) / offset;
    if (!Number.isFinite(ratio) || ratio <= 0) {
      return false;
    }
    knob.default_value = Math.log(ratio) / range;
  } else {
    knob.default_value = value;
  }
  return true;
}
