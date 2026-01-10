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

  function stepDecimals(step) {
    const text = String(step);
    if (text.includes(".")) {
      return text.split(".")[1].length;
    }
    return 0;
  }

  function formatValue(value, decimals) {
    return value.toFixed(decimals);
  }

  function notifyKnobChange(index, value) {
    if (typeof onChange === "function") {
      onChange(index, value);
    } else if (typeof window.onKnobChange === "function") {
      window.onKnobChange(index, value);
    }
  }

  knobConfigs.forEach(function(config, index) {
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
        notifyKnobChange(index, idx);
      });

      selectWrap.appendChild(select);
      col.appendChild(label);
      col.appendChild(selectWrap);
      grid.appendChild(col);
      return;
    }

    const min = parseNumber(config.min_value, 0);
    const max = parseNumber(config.max_value, 1);
    const defaultValue = parseNumber(config.default_value, min);
    const displayName = config.display_name || `Knob ${index + 1}`;
    const step = getStep(min, max);
    const decimals = stepDecimals(step);

    const col = document.createElement("div");
    col.className = colClass;

    const label = document.createElement("div");
    label.className = "small mb-2";
    label.textContent = displayName;

    const knob = document.createElement("div");
    knob.className = "amy-knob";
    knob.setAttribute("role", "slider");
    knob.setAttribute("aria-valuemin", String(min));
    knob.setAttribute("aria-valuemax", String(max));
    knob.setAttribute("tabindex", "0");

    const face = document.createElement("div");
    face.className = "amy-knob-face";

    const indicator = document.createElement("div");
    indicator.className = "amy-knob-indicator";

    face.appendChild(indicator);
    knob.appendChild(face);

    const number = document.createElement("input");
    number.className = "form-control form-control-sm knob-value mt-2 text-center";
    number.type = "number";
    number.min = String(min);
    number.max = String(max);
    number.step = String(step);

    col.appendChild(label);
    col.appendChild(knob);
    col.appendChild(number);
    grid.appendChild(col);

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

    function setValue(nextValue, commit) {
      const clamped = clamp(nextValue, min, max);
      state.value = clamped;
      const angle = valueToAngle(clamped);
      face.style.setProperty("--angle", `${angle}deg`);
      knob.setAttribute("aria-valuenow", formatValue(clamped, decimals));
      if (commit) {
        number.value = formatValue(clamped, decimals);
      }
      notifyKnobChange(index, clamped);
    }

    setValue(state.value, true);

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
      setValue(state.startValue + delta, true);
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
        const parsed = parseNumber(number.value, state.value);
        setValue(parsed, true);
      }
    });

    number.addEventListener("change", function() {
      const parsed = parseNumber(number.value, state.value);
      setValue(parsed, true);
    });

    number.addEventListener("blur", function() {
      const parsed = parseNumber(number.value, state.value);
      setValue(parsed, true);
    });

    knob.addEventListener("keydown", function(event) {
      if (event.key === "ArrowUp" || event.key === "ArrowRight") {
        event.preventDefault();
        setValue(state.value + step, true);
      }
      if (event.key === "ArrowDown" || event.key === "ArrowLeft") {
        event.preventDefault();
        setValue(state.value - step, true);
      }
    });
  });
}
