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

  function formatValue(value, digits) {
    if (!Number.isFinite(value)) {
      return "";
    }
    const totalDigits = Number.isFinite(digits) ? Math.max(1, Math.floor(digits)) : 3;
    const abs = Math.abs(value);
    if (abs === 0) {
      return (0).toFixed(Math.max(0, totalDigits - 1));
    }
    const intDigits = abs < 1 ? 1 : Math.floor(Math.log10(abs)) + 1;
    const decimals = Math.max(0, totalDigits - intDigits);
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
    const ccEditor = (function() {
      let editor = null;
      return function getCcEditor() {
        if (editor) {
          return editor;
        }
        const container = document.createElement("div");
        container.className = "cc-editor-popup";
        container.style.position = "absolute";
        container.style.zIndex = "9999";
        container.style.background = "#1f1f1f";
        container.style.color = "#fff";
        container.style.border = "1px solid #444";
        container.style.borderRadius = "8px";
        container.style.padding = "8px 10px";
        container.style.boxShadow = "0 10px 20px rgba(0, 0, 0, 0.35)";
        container.style.fontSize = "12px";
        container.style.display = "none";
        container.style.minWidth = "160px";

        const label = document.createElement("div");
        label.textContent = "MIDI CC (0-127)";
        label.style.marginBottom = "6px";

        const input = document.createElement("input");
        input.type = "text";
        input.style.width = "100%";
        input.style.boxSizing = "border-box";
        input.style.marginBottom = "6px";

        const error = document.createElement("div");
        error.style.color = "#f2c94c";
        error.style.fontSize = "11px";
        error.style.minHeight = "14px";
        error.style.marginBottom = "6px";

        const actions = document.createElement("div");
        actions.style.display = "flex";
        actions.style.gap = "6px";
        actions.style.justifyContent = "flex-end";

        const save = document.createElement("button");
        save.type = "button";
        save.textContent = "Save";
        save.style.fontSize = "11px";

        const cancel = document.createElement("button");
        cancel.type = "button";
        cancel.textContent = "Cancel";
        cancel.style.fontSize = "11px";

        actions.appendChild(cancel);
        actions.appendChild(save);
        container.appendChild(label);
        container.appendChild(input);
        container.appendChild(error);
        container.appendChild(actions);
        document.body.appendChild(container);

        editor = { container: container, input: input, error: error, save: save, cancel: cancel, current: null };

        function applyValue(value) {
          if (!editor.current) {
            return;
          }
          editor.current.cc = value;
          hideCcEditor(editor);
        }

        save.addEventListener("click", function() {
          const trimmed = editor.input.value.trim();
          if (trimmed === "") {
            applyValue("");
            return;
          }
          const parsed = Number(trimmed);
          if (!Number.isInteger(parsed) || parsed < 0 || parsed > 127) {
            editor.error.textContent = "Enter an integer 0-127.";
            return;
          }
          applyValue(parsed);
        });

        cancel.addEventListener("click", function() {
          hideCcEditor(editor);
        });

        input.addEventListener("keydown", function(event) {
          if (event.key === "Enter") {
            event.preventDefault();
            save.click();
          }
          if (event.key === "Escape") {
            event.preventDefault();
            hideCcEditor(editor);
          }
        });

        document.addEventListener("mousedown", function(event) {
          if (!editor.container.contains(event.target)) {
            hideCcEditor(editor);
          }
        });

        return editor;
      };
    })();

    function hideCcEditor(editor) {
      editor.container.style.display = "none";
      editor.current = null;
    }

    function positionCcEditor(editor, anchor) {
      const rect = anchor.getBoundingClientRect();
      const top = rect.bottom + window.scrollY + 6;
      const left = rect.left + window.scrollX;
      editor.container.style.top = `${top}px`;
      editor.container.style.left = `${left}px`;
    }

    function attachCcEditor(labelEl, knobConfig) {
      if (!labelEl) {
        return;
      }
      labelEl.style.cursor = "pointer";
      labelEl.setAttribute("title", "Click to edit MIDI CC");
      labelEl.addEventListener("click", function() {
        const editor = ccEditor();
        editor.current = knobConfig;
        editor.error.textContent = "";
        editor.input.value = knobConfig.cc === "" || knobConfig.cc === null || knobConfig.cc === undefined
          ? ""
          : String(knobConfig.cc);
        positionCcEditor(editor, labelEl);
        editor.container.style.display = "block";
        editor.input.focus();
        editor.input.select();
      });
    }

    if (config.knob_type === "selection") {
      const options = Array.isArray(config.options) ? config.options : [];
      const defaultIndex = parseNumber(config.default_value, 0);
      const clampedIndex = clamp(defaultIndex, 0, Math.max(options.length - 1, 0));
      const displayName = config.display_name || `Select ${index + 1}`;

      const col = document.createElement("div");
      col.className = colClass;
      col.style.setProperty("--knob-span", "1");

      const label = document.createElement("div");
      label.className = "knob-label small mb-2";
      label.textContent = displayName;
      attachCcEditor(label, config);

      const selectWrap = document.createElement("div");
      selectWrap.className = "amy-select";

      const control = document.createElement("div");
      control.className = "amy-select-control";
      control.setAttribute("data-index", String(index));
      control.setAttribute("tabindex", "0");

      const upButton = document.createElement("button");
      upButton.className = "amy-select-btn amy-select-btn-up";
      upButton.type = "button";
      upButton.setAttribute("aria-label", "Select previous option");

      const display = document.createElement("div");
      display.className = "amy-select-display";

      const downButton = document.createElement("button");
      downButton.className = "amy-select-btn amy-select-btn-down";
      downButton.type = "button";
      downButton.setAttribute("aria-label", "Select next option");

      let currentIndex = clampedIndex;

      function updateDisplay() {
        const optionLabel = options[currentIndex] !== undefined ? String(options[currentIndex]) : "";
        display.textContent = optionLabel;
      }

      function updateButtons() {
        const hasOptions = options.length > 0;
        const isFirst = !hasOptions || currentIndex <= 0;
        const isLast = !hasOptions || currentIndex >= options.length - 1;

        upButton.classList.toggle("is-disabled", isFirst);
        upButton.disabled = isFirst;
        upButton.setAttribute("aria-disabled", isFirst ? "true" : "false");

        downButton.classList.toggle("is-disabled", isLast);
        downButton.disabled = isLast;
        downButton.setAttribute("aria-disabled", isLast ? "true" : "false");
      }

      function setIndex(nextIndex) {
        const clamped = clamp(nextIndex, 0, Math.max(options.length - 1, 0));
        if (clamped === currentIndex) {
          return;
        }
        currentIndex = clamped;
        updateDisplay();
        updateButtons();
        notifyKnobChange(index, currentIndex, config, { notifyAmy: true });
      }

      upButton.addEventListener("click", function() {
        setIndex(currentIndex - 1);
      });

      downButton.addEventListener("click", function() {
        setIndex(currentIndex + 1);
      });

      control.addEventListener("keydown", function(event) {
        if (event.key === "ArrowUp" || event.key === "ArrowLeft") {
          event.preventDefault();
          setIndex(currentIndex - 1);
        }
        if (event.key === "ArrowDown" || event.key === "ArrowRight") {
          event.preventDefault();
          setIndex(currentIndex + 1);
        }
      });

      updateDisplay();
      updateButtons();

      control.appendChild(upButton);
      control.appendChild(display);
      control.appendChild(downButton);
      selectWrap.appendChild(control);
      col.appendChild(label);
      col.appendChild(selectWrap);
      targetGrid.appendChild(col);
      return;
    }

    if (config.knob_type === "spacer") {
      const col = document.createElement("div");
      col.className = `${colClass} knob-col-spacer`;
      col.style.setProperty("--knob-span", "1");

      const spacer = document.createElement("div");
      spacer.className = "knob-spacer";

      col.appendChild(spacer);
      targetGrid.appendChild(col);
      return;
    }

    if (config.knob_type === "spacer-half") {
      const col = document.createElement("div");
      col.className = `${colClass} knob-col-spacer-half`;
      col.style.setProperty("--knob-span", "0.5");
      col.style.flex = "0 0 calc(100% * 0.5 / var(--knob-units, var(--knob-count, 14)))";
      col.style.maxWidth = "calc(100% * 0.5 / var(--knob-units, var(--knob-count, 14)))";
      col.style.width = "calc(100% * 0.5 / var(--knob-units, var(--knob-count, 14)))";

      const spacer = document.createElement("div");
      spacer.className = "knob-spacer knob-spacer-half";

      col.appendChild(spacer);
      targetGrid.appendChild(col);
      return;
    }

    if (config.knob_type === "pushbutton") {
      const displayName = config.display_name || `Button ${index + 1}`;
      const defaultValue = parseNumber(config.default_value, 0);
      const state = {
        value: defaultValue === 1 ? 1 : 0
      };

      const col = document.createElement("div");
      col.className = `${colClass} knob-col-pushbutton`;
      col.style.setProperty("--knob-span", "0.5");

      const label = document.createElement("div");
      label.className = "knob-label small mb-2";
      label.textContent = displayName;
      attachCcEditor(label, config);

      const button = document.createElement("div");
      button.className = "amy-pushbutton";
      button.setAttribute("role", "button");
      button.setAttribute("tabindex", "0");
      if (config.color) {
        button.style.setProperty("--pushbutton-color", String(config.color));
      }

      const led = document.createElement("div");
      led.className = "amy-pushbutton-led";

      button.appendChild(led);
      col.appendChild(label);
      col.appendChild(button);
      targetGrid.appendChild(col);

      function setValue(nextValue, notifyAmy) {
        const clamped = nextValue === 1 ? 1 : 0;
        state.value = clamped;
        button.classList.toggle("is-on", clamped === 1);
        button.setAttribute("aria-pressed", clamped === 1 ? "true" : "false");
        notifyKnobChange(index, clamped, config, { notifyAmy: notifyAmy });
      }

      function toggle() {
        setValue(state.value === 1 ? 0 : 1, true);
      }

      setValue(state.value, false);

      button.addEventListener("pointerdown", function(event) {
        event.preventDefault();
        toggle();
      });

      button.addEventListener("keydown", function(event) {
        if (event.key === "Enter" || event.key === " ") {
          event.preventDefault();
          toggle();
        }
      });

      return;
    }

    const isLogKnob = config.knob_type === "log";
    const logOffset = parseNumber(config.offset, 0);
    const min = parseNumber(config.min_value, 0);
    const max = parseNumber(config.max_value, 1);
    const defaultValue = parseNumber(config.default_value, min);
    const displayName = config.display_name || `Knob ${index + 1}`;
    const knobMin = isLogKnob ? 0 : min;
    const knobMax = isLogKnob ? 1 : max;
    const step = getStep(knobMin, knobMax);
    const displayDecimals = 3;

    const col = document.createElement("div");
    col.className = colClass;
    col.style.setProperty("--knob-span", "1");

    const label = document.createElement("div");
    label.className = "knob-label small mb-2";
    label.textContent = displayName;
    attachCcEditor(label, config);

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
      const minShifted = min + logOffset;
      const maxShifted = max + logOffset;
      if (!Number.isFinite(minShifted) || !Number.isFinite(maxShifted) || minShifted <= 0 || maxShifted <= 0) {
        return min;
      }
      const ratio = maxShifted / minShifted;
      return minShifted * Math.pow(2, value * Math.log2(ratio)) - logOffset;
    }

    const displayRange = isLogKnob ? { min: min, max: max } : { min: min, max: max };

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
      dragStep: Math.abs(knobMax - knobMin) / 150
    };

    function valueToAngle(value) {
      if (knobMax === knobMin) return angleStart;
      const normalized = (value - knobMin) / (knobMax - knobMin);
      return angleStart + clamp(normalized, 0, 1) * angleSpan;
    }

    function outputToValue(output) {
      if (!Number.isFinite(output)) {
        return state.value;
      }
      const minShifted = min + logOffset;
      const maxShifted = max + logOffset;
      if (!Number.isFinite(minShifted) || !Number.isFinite(maxShifted) || minShifted <= 0 || maxShifted <= 0) {
        return state.value;
      }
      const ratio = (output + logOffset) / minShifted;
      if (!Number.isFinite(ratio) || ratio <= 0) {
        return state.value;
      }
      const denom = Math.log2(maxShifted / minShifted);
      if (!Number.isFinite(denom) || denom === 0) {
        return state.value;
      }
      const raw = Math.log2(ratio) / denom;
      if (!Number.isFinite(raw)) {
        return state.value;
      }
      return clamp(raw, 0, 1);
    }

    function setValue(nextValue, commit, notifyAmy) {
      const clamped = isLogKnob ? clamp(nextValue, 0, 1) : clamp(nextValue, min, max);
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

    if (isLogKnob) {
      const defaultShifted = defaultValue + logOffset;
      const minShifted = min + logOffset;
      const maxShifted = max + logOffset;
      if (Number.isFinite(defaultShifted) && Number.isFinite(minShifted) && Number.isFinite(maxShifted)
        && minShifted > 0 && maxShifted > 0 && defaultShifted > 0) {
        const denom = Math.log2(maxShifted / minShifted);
        state.value = denom === 0 ? 0 : Math.log2(defaultShifted / minShifted) / denom;
      } else {
        state.value = 0;
      }
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
  const sectionStyles = Array.isArray(window.amy_sections)
    ? window.amy_sections.reduce(function(map, entry) {
      if (entry && typeof entry.name === "string") {
        map[entry.name] = entry;
      }
      return map;
    }, {})
    : {};

  knobConfigs.forEach(function(config, index) {
    const sectionName = typeof config.section === "string" ? config.section : "";
    if (!currentSection || currentSection.name !== sectionName) {
      currentSection = { name: sectionName, items: [], units: 0 };
      sections.push(currentSection);
    }
    currentSection.items.push({ config: config, index: index });
    if (config.knob_type === "pushbutton" || config.knob_type === "spacer-half") {
      currentSection.units += 0.5;
    } else {
      currentSection.units += 1;
    }
  });

  sections.forEach(function(section) {
    const sectionWrap = document.createElement("div");
    const sectionClass = targetId === "knob-grid" ? "col-12 knob-section knob-section-main" : "col-12 knob-section";
    sectionWrap.className = sectionClass;
    sectionWrap.style.setProperty("--knob-count", String(section.items.length));
    const sectionUnits = section.units || section.items.length;
    const rowUnits = targetId === "knob-grid" ? Math.min(14, sectionUnits) : sectionUnits;
    sectionWrap.style.setProperty("--knob-units", String(rowUnits));
    const styleConfig = sectionStyles[section.name];
    if (styleConfig && styleConfig.bg_color) {
      sectionWrap.style.background = styleConfig.bg_color;
    }

    const showHeader = section.name && section.items.some(function(item) {
      return item.config && item.config.knob_type !== "spacer" && item.config.knob_type !== "spacer-half";
    });

    if (showHeader) {
      const header = document.createElement("div");
      header.className = "knob-section-header";
      header.textContent = section.name || "";
      if (styleConfig) {
        if (styleConfig.header_bg_color) {
          header.style.background = styleConfig.header_bg_color;
        }
        if (styleConfig.header_fg_color) {
          header.style.color = styleConfig.header_fg_color;
        }
      }
      sectionWrap.appendChild(header);
    }

    const sectionGrid = document.createElement("div");
    sectionGrid.className = targetId === "knob-grid" ? "row g-4 knob-section-grid" : "row g-3 knob-section-grid";

    sectionWrap.appendChild(sectionGrid);
    grid.appendChild(sectionWrap);

    section.items.forEach(function(item) {
      renderKnob(item.config, item.index, sectionGrid);
    });
  });
}

function set_amy_knob_value(knobs, sectionName, name, value) {
  if (!Array.isArray(knobs) || typeof sectionName !== "string" || typeof name !== "string" || !Number.isFinite(value)) {
    return false;
  }
  const knob = knobs.find((entry) => entry.section === sectionName && entry.display_name === name);
  if (!knob) {
    return false;
  }
  if (knob.knob_type === "log") {
    const min = Number(knob.min_value);
    const max = Number(knob.max_value);
    const offset = Number(knob.offset || 0);
    if (!Number.isFinite(min) || !Number.isFinite(max) || !Number.isFinite(offset)) {
      return false;
    }
    const minShifted = min + offset;
    const maxShifted = max + offset;
    if (minShifted <= 0 || maxShifted <= 0) {
      return false;
    }
    knob.default_value = value;
  } else {
    knob.default_value = value;
  }
  knob.onChange(value);
  return true;
}
