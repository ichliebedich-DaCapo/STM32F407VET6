#!/usr/bin/env python3
import os
import re
import glob
import argparse



def parse_setup_function(content):
    pattern = r'void setup_scr_(\w+)\(lv_ui \*ui\)\s*{([^}]+)}'
    match = re.search(pattern,  content, re.DOTALL)
    if not match:
        raise ValueError("Could not find setup_scr_* function")
    return match.group(1),  match.group(2).strip()

def convert_style_calls(lines, widget_var):
    converted = []
    for line in lines:
        line = line.strip()
        if not line:
            continue

            # Match style setting functions
        match = re.match(r'lv_obj_set_style_(\w+)\(ui->\w+,\s*(.+?),\s*(LV_PART_\w+\ |LV_STATE_\w+)\);', line)
        if match:
            prop = match.group(1)
            value = match.group(2)
            state = match.group(3)

            # Handle default state
            param = "" if state == "LV_PART_MAIN|LV_STATE_DEFAULT" else f", {state}"
            converted.append(f"{widget_var}.{prop}({value}{param})")
    return converted

def process_component_block(component_name, create_line, init_lines, style_lines, widget_type):
    # Determine prefix based on creation function
    prefix_map = {
        "lv_obj_create": "obj_",
        "lv_imagebutton_create": "imgbtn_",
        "lv_label_create": "label_",
        "lv_image_create": "img_"
    }

    prefix = "obj_"
    for func, p in prefix_map.items():
        if func in create_line:
            prefix = p
            break

    var_name = prefix + component_name.split('_')[-1]

    # Process initialization lines
    init_code = []
    pos = size = None
    for line in init_lines:
        if "lv_obj_set_pos" in line:
            pos = re.findall(r'\d+',  line)
        elif "lv_obj_set_size" in line:
            size = re.findall(r'\d+',  line)
        elif "lv_obj_add_flag" in line:
            flag = line.split(',')[-1].strip().rstrip(');')
            init_code.append(f".add_flag({flag})")

            # Build init chain
    init_chain = []
    if pos and size:
        init_chain.append(f".init({',  '.join(pos + size)})")
    init_chain.extend(init_code)

    # Process styles
    style_code = convert_style_calls(style_lines, var_name)

    return {
        "var_name": var_name,
        "init_chain": init_chain,
        "style_code": style_code,
        "widget_type": widget_type
    }

def generate_output(project_name, components):
    widgets_ns = []
    screen_ns = []
    for comp in components:
        # Widget declaration
        widgets_ns.append(f"Component  {comp['var_name']};")

        # Screen setup code
        screen_code = [
            f"auto& {comp['var_name']} = widgets::{comp['var_name']};"
        ]
        if comp['init_chain']:
            screen_code.append(
                f"{comp['var_name']}" + "".join(comp['init_chain']) + ";"
            )
        screen_code.extend(comp['style_code'])

        screen_ns.extend(screen_code)

    widgets_block = "\n".join(widgets_ns)
    screen_block = "\n    ".join(screen_ns)

    return f"""namespace gui::widgets::main 
{{
{widgets_block}
}}

namespace gui::init 
{{
    void screen()
    {{
        using namespace gui::widgets::main;
        {screen_block}
    }}
}}"""

def main():
    # 确定文件路径
    parser = argparse.ArgumentParser()
    parser.add_argument("--path",  default="../Projects/driversDevelop/ui/generated")
    args = parser.parse_args()

    # 寻找目标文件
    target_files = glob.glob(os.path.join(args.path,  "setup_scr_*.c"))
    if not target_files:
        raise FileNotFoundError("No setup files found")
    if len(target_files) > 1:
        raise ValueError("Multiple setup files found")

    with open(target_files[0]) as f:
        content = f.read()

    # 定位 setup_scr_* 函数
    project_name, func_body = parse_setup_function(content)

    # Process code blocks
    blocks = re.split(r'(//Write {2}codes? \w+)', func_body)
    components = []
    current_component = None

    for i in range(1, len(blocks), 2):
        comment_line = blocks[i].strip()
        code_block = blocks[i+1].strip()

        component_type = "main" if "style for" in comment_line else "child"
        component_name = comment_line.split()[-1]

        if component_type == "main":
            # Skip first component's code
            if "Write codes " in comment_line and not components:
                continue

                # Process style block
            style_lines = [line for line in code_block.split('\n')  if line.strip()]
            components[-1]["style_code"].extend(
                convert_style_calls(style_lines, "scr")
            )
        else:
            # New component block
            parts = code_block.split('//Write  style for', 1)
            creation_block = parts[0].strip().split('\n')
            style_block = parts[1].split('//Write codes', 1)[0].strip() if len(parts) > 1 else ""

            # Process component
            component = process_component_block(
                component_name,
                creation_block[0],
                creation_block[1:-1],
                style_block.split('\n'),
                "lv_obj_t" if "lv_obj_create" in creation_block[0] else "other"
            )
            components.append(component)


    # 输出结果
    output = generate_output(project_name, components)

    with open("ui.cpp",  "w") as f:
        f.write(output)

if __name__ == "__main__":
    main()