#!/usr/bin/env python3
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
AMY_H = ROOT / "amy" / "src" / "amy.h"
LAYOUT_OUTPUT = ROOT / "tulip" / "amyboardweb" / "static" / "amy_event_layout.generated.js"
PATCHES_OUTPUT = ROOT / "tulip" / "amyboardweb" / "static" / "patches.generated.js"
PATCHES_H = ROOT / "amy" / "src" / "patches.h"

TYPE_INFO = {
    "uint32_t": (4, 4),
    "int32_t": (4, 4),
    "uint16_t": (2, 2),
    "int16_t": (2, 2),
    "uint8_t": (1, 1),
    "int8_t": (1, 1),
    "float": (4, 4),
    "char": (1, 1),
}

NEEDED_CONSTANTS = [
    "NUM_COMBO_COEFS",
    "MAX_BREAKPOINT_SETS",
    "MAX_ALGO_OPS",
    "MAX_VOICES_PER_INSTRUMENT",
    "MAX_PARAM_LEN",
]

DEFAULT_CONSTANTS = {
    "MAX_EVENTS_PER_PATCH": 10,
}


def strip_comments(line: str) -> str:
    if "/*" in line and "*/" in line:
        pre = line.split("/*", 1)[0]
        post = line.split("*/", 1)[1]
        line = pre + post
    if "//" in line:
        line = line.split("//", 1)[0]
    return line.strip()


def parse_constants(text: str) -> dict:
    constants = {}
    for line in text.splitlines():
        line = line.strip()
        if not line.startswith("#define"):
            continue
        parts = line.split()
        if len(parts) < 3:
            continue
        name = parts[1]
        if name in NEEDED_CONSTANTS:
            value = parts[2]
            if value.isdigit():
                constants[name] = int(value)
    for name in NEEDED_CONSTANTS:
        if name not in constants:
            raise RuntimeError(f"Missing constant {name} in {AMY_H}")
    constants.update(DEFAULT_CONSTANTS)
    return constants


def parse_struct_fields(text: str):
    start = text.find("typedef struct amy_event")
    if start == -1:
        raise RuntimeError("Could not find amy_event struct")
    block = text[start:]
    block_start = block.find("{")
    block_end = block.find("} amy_event;")
    if block_start == -1 or block_end == -1:
        raise RuntimeError("Could not parse amy_event struct block")
    body = block[block_start + 1:block_end]
    fields = []
    for raw_line in body.splitlines():
        line = strip_comments(raw_line)
        if not line:
            continue
        match = re.match(r"^(\w+)\s+(\w+)(?:\[([^\]]+)\])?\s*;$", line)
        if not match:
            continue
        type_token = match.group(1).strip()
        if type_token not in TYPE_INFO:
            raise RuntimeError(f"Unknown type {type_token} in line: {line}")
        array_part = match.group(3)
        count = 1
        if array_part:
            inner = array_part.strip()
            if inner.isdigit():
                count = int(inner)
            else:
                count = inner
        fields.append((match.group(2), type_token, count))
    return fields


def compute_layout(fields, constants):
    offsets = {}
    offset = 0
    struct_align = 1
    for name, type_token, count in fields:
        size, align = TYPE_INFO[type_token]
        struct_align = max(struct_align, align)
        if offset % align:
            offset += align - (offset % align)
        offsets[name] = offset
        if isinstance(count, str):
            if count not in constants:
                raise RuntimeError(f"Unknown array constant {count} for {name}")
            count_value = constants[count]
        else:
            count_value = count
        offset += size * count_value
    if offset % struct_align:
        offset += struct_align - (offset % struct_align)
    return offset, offsets


def generate_layout():
    text = AMY_H.read_text()
    constants = parse_constants(text)
    fields = parse_struct_fields(text)
    size, offsets = compute_layout(fields, constants)

    payload = {
        "size": size,
        "offsets": offsets,
        "constants": constants,
    }
    output = "// Auto-generated from amy/src/amy.h\n"
    output += "window.AMY_EVENT_LAYOUT = "
    output += json.dumps(payload, indent=2, sort_keys=True)
    output += ";\n"
    LAYOUT_OUTPUT.write_text(output)


def generate_patches():
    text = PATCHES_H.read_text()
    matches = re.findall(r"/\*\s*(\d+)\s*:\s*(.*?)\s*\*/", text)
    patches = [(int(num), name) for num, name in matches]
    patches.sort(key=lambda item: item[0])
    if not patches:
        raise RuntimeError("No patch names found in patches.h")
    for idx, (num, _) in enumerate(patches):
        if num != idx:
            raise RuntimeError(f"Missing patch index {idx} in patches.h")
    names = [name for _, name in patches]
    output = "// Auto-generated from amy/src/patches.h\n"
    output += "(function() {\n"
    output += "  const patches = [\n"
    for name in names:
        output += f"    {json.dumps(name)},\n"
    output += "  ];\n\n"
    output += "  window.amy_patches = patches;\n\n"
    output += "  if (typeof window.onPatchChange !== \"function\") {\n"
    output += "    window.onPatchChange = function(_patchIndex) {};\n"
    output += "  }\n\n"
    output += "  window.init_patches_select = function(selectId) {\n"
    output += "    const select = document.getElementById(selectId);\n"
    output += "    if (!select) {\n"
    output += "      return;\n"
    output += "    }\n"
    output += "    select.innerHTML = \"\";\n"
    output += "    patches.forEach(function(name, index) {\n"
    output += "      const option = document.createElement(\"option\");\n"
    output += "      option.value = String(index);\n"
    output += "      option.textContent = name;\n"
    output += "      select.appendChild(option);\n"
    output += "    });\n\n"
    output += "  };\n"
    output += "})();\n"
    PATCHES_OUTPUT.write_text(output)


def main():
    generate_layout()
    generate_patches()


if __name__ == "__main__":
    main()
