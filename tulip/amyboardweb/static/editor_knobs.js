function make_change_code(synth, value, knob, no_instrument) {
  if (!knob || typeof knob.change_code !== "string") {
    return;
  }
  if (Math.round(value) == value) {
    var valueStr = String(value);
  } else {
    var valueStr = value.toFixed(3);
  }
  let updated = knob.change_code.replace(/%v/g, valueStr);
  if (no_instrument) {
    updated = updated.replace(/i%i/g, "");
  } else {
    updated = updated.replace(/%i/g, String(synth));
  }
  const selectedPreset = get_wave_preset_for_value(knob, value, { allowFallback: false });
  if (selectedPreset !== null) {
    updated = updated.split("Z").map(function(part) {
      if (!part) {
        return part;
      }
      return part + "p" + String(selectedPreset);
    }).join("Z");
  }
  return updated;
}

function send_change_code(synth, value, knob) {
  if (typeof window.amy_add_message === "function") {
    window.amy_add_log_message(make_change_code(synth, value, knob));
  }
}

function set_knob_ui_value(knob, value, notifyAmy) {
  if (!knob || typeof knob._setValue !== "function") {
    return false;
  }
  knob._setValue(value, notifyAmy === true);
  return true;
}

function get_wave_preset_options_for_value(waveValue) {
  const allPresets = (typeof window !== "undefined" && window.AMY_WAVE_PRESETS && typeof window.AMY_WAVE_PRESETS === "object")
    ? window.AMY_WAVE_PRESETS
    : null;
  if (!allPresets) {
    return [];
  }
  const key = String(Number(waveValue));
  const options = allPresets[key];
  if (!Array.isArray(options)) {
    return [];
  }
  return options;
}

function ensure_wave_preset_storage(knob) {
  if (!knob || typeof knob !== "object") {
    return null;
  }
  if (!knob.wave_preset_by_wave || typeof knob.wave_preset_by_wave !== "object") {
    knob.wave_preset_by_wave = {};
  }
  return knob.wave_preset_by_wave;
}

function get_wave_preset_for_value(knob, waveValue, opts) {
  const optionsArg = (opts && typeof opts === "object") ? opts : {};
  const allowFallback = optionsArg.allowFallback === true;
  if (!knob || knob.knob_type !== "selection" || knob.display_name !== "wave") {
    return null;
  }
  const options = get_wave_preset_options_for_value(waveValue);
  if (!options.length) {
    return null;
  }
  const waveKey = String(Number(waveValue));
  const stored = knob.wave_preset_by_wave && knob.wave_preset_by_wave[waveKey];
  const storedNum = Number(stored);
  if (Number.isInteger(storedNum) && options.some(function(entry) { return Number(entry.value) === storedNum; })) {
    return storedNum;
  }
  if (allowFallback) {
    const first = Number(options[0].value);
    if (Number.isInteger(first)) {
      return first;
    }
  }
  return null;
}

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
    if (config && typeof config === "object") {
      config.default_value = value;
    }
    const notifyAmy = !options || options.notifyAmy !== false;
    if (notifyAmy && !window.suppress_knob_cc_send) {
      send_change_code(window.current_synth, value, config);
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
        container.style.padding = "6px 8px";
        container.style.boxShadow = "0 10px 20px rgba(0, 0, 0, 0.35)";
        container.style.fontSize = "12px";
        container.style.display = "none";
        container.style.minWidth = "120px";

        const label = document.createElement("div");
        label.textContent = "MIDI CC (0-127)";
        const title = document.createElement("div");
        title.style.fontWeight = "600";
        title.style.marginBottom = "4px";
        label.style.marginBottom = "6px";

        const rangeWrap = document.createElement("div");
        rangeWrap.style.display = "none";
        rangeWrap.style.marginBottom = "6px";

        const rangeRow = document.createElement("div");
        rangeRow.style.display = "flex";
        rangeRow.style.gap = "6px";
        rangeRow.style.marginBottom = "6px";

        const minField = document.createElement("div");
        minField.style.flex = "1";
        const minLabel = document.createElement("div");
        minLabel.textContent = "min";
        minLabel.style.fontSize = "11px";
        minLabel.style.marginBottom = "2px";
        const minInput = document.createElement("input");
        minInput.type = "text";
        minInput.style.width = "60px";
        minInput.style.boxSizing = "border-box";
        minInput.style.height = "18px";
        minInput.style.padding = "1px 4px";
        minInput.style.fontSize = "11px";
        minField.appendChild(minLabel);
        minField.appendChild(minInput);

        const maxField = document.createElement("div");
        maxField.style.flex = "1";
        const maxLabel = document.createElement("div");
        maxLabel.textContent = "max";
        maxLabel.style.fontSize = "11px";
        maxLabel.style.marginBottom = "2px";
        const maxInput = document.createElement("input");
        maxInput.type = "text";
        maxInput.style.width = "60px";
        maxInput.style.boxSizing = "border-box";
        maxInput.style.height = "18px";
        maxInput.style.padding = "1px 4px";
        maxInput.style.fontSize = "11px";
        maxField.appendChild(maxLabel);
        maxField.appendChild(maxInput);

        rangeRow.appendChild(minField);
        rangeRow.appendChild(maxField);

        const logRow = document.createElement("label");
        logRow.style.display = "flex";
        logRow.style.alignItems = "center";
        logRow.style.gap = "6px";
        logRow.style.fontSize = "11px";
        const logCheckbox = document.createElement("input");
        logCheckbox.type = "checkbox";
        const logText = document.createElement("span");
        logText.textContent = "log";
        logRow.appendChild(logCheckbox);
        logRow.appendChild(logText);

        rangeWrap.appendChild(rangeRow);
        rangeWrap.appendChild(logRow);

        const input = document.createElement("input");
        input.type = "text";
        input.style.width = "60px";
        input.style.boxSizing = "border-box";
        input.style.marginBottom = "6px";
        input.style.height = "18px";
        input.style.padding = "1px 4px";
        input.style.fontSize = "11px";

        const error = document.createElement("div");
        error.style.color = "#f2c94c";
        error.style.fontSize = "11px";
        error.style.minHeight = "14px";
        error.style.marginBottom = "6px";

        const learn = document.createElement("button");
        learn.type = "button";
        learn.textContent = "Learn";
        learn.style.fontSize = "11px";
        learn.style.marginBottom = "6px";

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
        container.appendChild(title);
        container.appendChild(rangeWrap);
        container.appendChild(label);
        container.appendChild(input);
        container.appendChild(learn);
        container.appendChild(error);
        container.appendChild(actions);
        document.body.appendChild(container);

        editor = { container: container, input: input, error: error, save: save, cancel: cancel, learn: learn, current: null, title: title, learning: false, minInput: minInput, maxInput: maxInput, logCheckbox: logCheckbox, rangeWrap: rangeWrap };

        function setLearnActive(active) {
          editor.learning = active === true;
          if (editor.learn) {
            editor.learn.style.backgroundColor = editor.learning ? "#1fd17a" : "";
            editor.learn.style.color = editor.learning ? "#111" : "";
          }
          if (!editor.learning && window.cc_learn_handler) {
            window.cc_learn_handler = null;
          }
        }

        function parseNumberField(fieldValue) {
          if (fieldValue.trim() === "") {
            return null;
          }
          const parsed = Number(fieldValue);
          return Number.isFinite(parsed) ? parsed : null;
        }

        function applyRangeUpdates() {
          if (!editor.rangeWrap || editor.rangeWrap.style.display === "none") {
            return true;
          }
          const minValue = parseNumberField(editor.minInput.value);
          const maxValue = parseNumberField(editor.maxInput.value);
          if (minValue === null || maxValue === null) {
            editor.error.textContent = "Enter numeric min/max values.";
            return false;
          }
          if (minValue >= maxValue) {
            editor.error.textContent = "min_value must be less than max_value.";
            return false;
          }
          const prevMin = editor.current.min_value;
          const prevMax = editor.current.max_value;
          const prevType = editor.current.knob_type;
          editor.current.min_value = minValue;
          editor.current.max_value = maxValue;
          if (editor.logCheckbox && editor.logCheckbox.checked) {
            editor.current.knob_type = "log";
          } else if (editor.current.knob_type === "log") {
            editor.current.knob_type = undefined;
          }
          const rangeChanged = prevMin !== editor.current.min_value || prevMax !== editor.current.max_value;
          const typeChanged = prevType !== editor.current.knob_type;
          if (rangeChanged || typeChanged) {
            if (typeof window.refresh_knobs_for_active_channel === "function") {
              window.refresh_knobs_for_active_channel();
            }
          }
          return true;
        }

        function applyValue(value) {
          if (!editor.current) {
            return;
          }
          editor.current.cc = value;
          if (typeof window.onKnobCcChange === "function") {
            window.onKnobCcChange(editor.current);
          }
          hideCcEditor(editor);
        }

        save.addEventListener("click", function() {
          const trimmed = editor.input.value.trim();
          if (trimmed === "") {
            if (!applyRangeUpdates()) {
              return;
            }
            applyValue("");
            return;
          }
          const parsed = Number(trimmed);
          if (!Number.isInteger(parsed) || parsed < 0 || parsed > 127) {
            editor.error.textContent = "Enter an integer 0-127.";
            return;
          }
          if (!applyRangeUpdates()) {
            return;
          }
          applyValue(parsed);
        });

        cancel.addEventListener("click", function() {
          hideCcEditor(editor);
        });

        learn.addEventListener("click", function() {
          if (!editor.current) {
            return;
          }
          if (editor.learning) {
            setLearnActive(false);
            return;
          }
          setLearnActive(true);
          window.cc_learn_handler = function(cc) {
            if (!editor.current) {
              setLearnActive(false);
              return;
            }
            editor.input.value = String(cc);
            editor.error.textContent = "";
            setLearnActive(false);
            editor.input.focus();
            editor.input.select();
          };
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
      if (editor && editor.learning) {
        editor.learning = false;
        if (editor.learn) {
          editor.learn.style.backgroundColor = "";
          editor.learn.style.color = "";
        }
        if (window.cc_learn_handler) {
          window.cc_learn_handler = null;
        }
      }
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
        const sectionName = knobConfig.section ? String(knobConfig.section) : "Knob";
        const displayName = knobConfig.display_name ? String(knobConfig.display_name) : "";
        editor.title.textContent = `${sectionName} ${displayName}`.trim();
        editor.input.value = knobConfig.cc === "" || knobConfig.cc === null || knobConfig.cc === undefined
          ? ""
          : String(knobConfig.cc);
        const isRangeType = !knobConfig.knob_type || knobConfig.knob_type === "log";
        editor.rangeWrap.style.display = isRangeType ? "block" : "none";
        if (isRangeType) {
          const minValue = Number.isFinite(Number(knobConfig.min_value)) ? String(knobConfig.min_value) : "";
          const maxValue = Number.isFinite(Number(knobConfig.max_value)) ? String(knobConfig.max_value) : "";
          editor.minInput.value = minValue;
          editor.maxInput.value = maxValue;
          editor.logCheckbox.checked = knobConfig.knob_type === "log";
        }
        positionCcEditor(editor, labelEl);
        editor.container.style.display = "block";
        editor.input.focus();
        editor.input.select();
      });
    }

    if (config.knob_type === "selection") {
      const options = Array.isArray(config.options) ? config.options : [];
      const optionValues = Array.isArray(config.option_values) ? config.option_values : null;

      function valueForIndex(idx) {
        if (optionValues && optionValues[idx] !== undefined) {
          const parsed = parseNumber(optionValues[idx], idx);
          return parsed;
        }
        return idx;
      }

      function indexForValue(rawValue, fallbackIndex) {
        const parsedValue = parseNumber(rawValue, fallbackIndex);
        if (optionValues) {
          const mappedIndex = optionValues.findIndex(function(mappedValue) {
            return parseNumber(mappedValue, NaN) === parsedValue;
          });
          if (mappedIndex >= 0) {
            return mappedIndex;
          }
        }
        return parsedValue;
      }

      const defaultIndex = indexForValue(config.default_value, 0);
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

      const presetPopup = (function() {
        let popup = null;
        let currentAnchor = null;
        function closePopup() {
          if (!popup) {
            return;
          }
          popup.style.display = "none";
          popup.setAttribute("data-wave", "");
          currentAnchor = null;
        }
        function openPopup(anchorEl, waveValue, onSelectPreset, currentPreset) {
          const presetOptions = get_wave_preset_options_for_value(waveValue);
          if (!presetOptions.length) {
            return;
          }
          if (!popup) {
            popup = document.createElement("div");
            popup.className = "amy-wave-preset-popup";
            popup.style.display = "none";
            document.body.appendChild(popup);
            document.addEventListener("mousedown", function(event) {
              if (!popup || popup.style.display === "none") {
                return;
              }
              if (popup.contains(event.target) || (currentAnchor && currentAnchor.contains(event.target))) {
                return;
              }
              closePopup();
            });
          }

          popup.innerHTML = "";
          const title = document.createElement("div");
          title.className = "amy-wave-preset-popup-title";
          title.textContent = Number(waveValue) === 19 ? "Wavetable Presets" : "PCM Presets";
          popup.appendChild(title);

          for (const preset of presetOptions) {
            const presetValue = Number(preset.value);
            if (!Number.isInteger(presetValue)) {
              continue;
            }
            const button = document.createElement("button");
            button.type = "button";
            button.className = "amy-wave-preset-option";
            if (Number(currentPreset) === presetValue) {
              button.classList.add("is-selected");
            }
            const labelName = preset.name !== undefined ? String(preset.name) : "";
            button.textContent = String(presetValue) + ": " + labelName;
            button.addEventListener("click", function() {
              onSelectPreset(presetValue);
              closePopup();
            });
            popup.appendChild(button);
          }

          popup.style.display = "block";
          popup.setAttribute("data-wave", String(waveValue));
          currentAnchor = anchorEl;
          const rect = anchorEl.getBoundingClientRect();
          popup.style.left = `${rect.left + window.scrollX}px`;
          popup.style.top = `${rect.bottom + window.scrollY + 6}px`;
        }
        return { openPopup: openPopup, closePopup: closePopup };
      })();

      function currentWaveValue() {
        return valueForIndex(currentIndex);
      }

      function setWavePresetForWave(waveValue, presetValue, notifyAmy) {
        const presetOptions = get_wave_preset_options_for_value(waveValue);
        if (!presetOptions.length) {
          return false;
        }
        const parsedPreset = Number(presetValue);
        if (!Number.isInteger(parsedPreset)) {
          return false;
        }
        const valid = presetOptions.some(function(entry) {
          return Number(entry.value) === parsedPreset;
        });
        if (!valid) {
          return false;
        }
        const storage = ensure_wave_preset_storage(config);
        if (!storage) {
          return false;
        }
        storage[String(Number(waveValue))] = parsedPreset;
        if (notifyAmy && !window.suppress_knob_cc_send) {
          send_change_code(window.current_synth, currentWaveValue(), config);
        }
        return true;
      }

      function updateDisplay() {
        const optionLabel = options[currentIndex] !== undefined ? String(options[currentIndex]) : "";
        display.textContent = optionLabel;
        const waveValue = currentWaveValue();
        const hasPresetPopup = get_wave_preset_options_for_value(waveValue).length > 0;
        display.classList.toggle("amy-select-display-clickable", hasPresetPopup);
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

      function setIndex(nextIndex, notifyAmy) {
        const clamped = clamp(nextIndex, 0, Math.max(options.length - 1, 0));
        if (clamped === currentIndex) {
          return;
        }
        currentIndex = clamped;
        updateDisplay();
        updateButtons();
        notifyKnobChange(index, valueForIndex(currentIndex), config, { notifyAmy: notifyAmy !== false });
      }

      upButton.addEventListener("click", function() {
        setIndex(currentIndex - 1, true);
      });

      downButton.addEventListener("click", function() {
        setIndex(currentIndex + 1, true);
      });

      control.addEventListener("keydown", function(event) {
        if (event.key === "ArrowUp" || event.key === "ArrowLeft") {
          event.preventDefault();
          setIndex(currentIndex - 1, true);
        }
        if (event.key === "ArrowDown" || event.key === "ArrowRight") {
          event.preventDefault();
          setIndex(currentIndex + 1, true);
        }
      });

      display.addEventListener("click", function(event) {
        const waveValue = currentWaveValue();
        const presetOptions = get_wave_preset_options_for_value(waveValue);
        if (!presetOptions.length) {
          return;
        }
        event.preventDefault();
        const selected = get_wave_preset_for_value(config, waveValue, { allowFallback: false });
        presetPopup.openPopup(
          display,
          waveValue,
          function(presetValue) {
            setWavePresetForWave(waveValue, presetValue, true);
          },
          selected
        );
      });

      updateDisplay();
      updateButtons();

      config._setValue = function(nextValue, notifyAmy) {
        const parsed = indexForValue(nextValue, currentIndex);
        setIndex(parsed, notifyAmy);
      };
      config._setWavePreset = function(waveValue, presetValue, notifyAmy) {
        return setWavePresetForWave(waveValue, presetValue, notifyAmy === true);
      };
      config._getWavePreset = function(waveValue) {
        return get_wave_preset_for_value(config, waveValue, { allowFallback: false });
      };

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

      config._setValue = function(nextValue, notifyAmy) {
        setValue(nextValue, notifyAmy);
      };

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

    config._setValue = function(outputValue, notifyAmy) {
      const parsed = parseNumber(outputValue, state.value);
      const rawValue = isLogKnob ? outputToValue(parsed) : parsed;
      setValue(rawValue, true, notifyAmy);
    };
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
  if (!window.suppress_knob_cc_send) {
    send_change_code(window.current_synth, value, knob);
  }
  return true;
}

function set_amy_knob_wave_preset(knobs, sectionName, waveValue, presetValue, notifyAmy) {
  if (!Array.isArray(knobs) || typeof sectionName !== "string") {
    return false;
  }
  const knob = knobs.find(function(entry) {
    return entry && entry.section === sectionName && entry.display_name === "wave";
  });
  if (!knob) {
    return false;
  }
  const presetNum = Number(presetValue);
  const waveNum = Number(waveValue);
  if (!Number.isInteger(presetNum) || !Number.isInteger(waveNum)) {
    return false;
  }
  const storage = ensure_wave_preset_storage(knob);
  if (!storage) {
    return false;
  }
  storage[String(waveNum)] = presetNum;
  if (typeof knob._setWavePreset === "function") {
    knob._setWavePreset(waveNum, presetNum, notifyAmy === true);
  } else if (notifyAmy && !window.suppress_knob_cc_send) {
    send_change_code(window.current_synth, waveNum, knob);
  }
  return true;
}
