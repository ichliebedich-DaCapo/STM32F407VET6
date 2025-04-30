# 根据字典组织信息，格式如下：
# {
#   "/path/to/file1.c": {
#     "variables": [
#       "inline Button btn_press;"
#     ],
#     "calls": {
#       "press": {
#         "chain": [
#           "btn_press.init(screen)"
#         ],
#         "lvgl": [
#           "lv_obj_set_size(btn_press,100,20);",
#           "lv_obj_set_text_size(btn_press,100,20);"
#         ]
#       }
#     }
#   },
#   {
#       ……
#   }
# }
# 遍历并组织字典的信息，把它变成这样的代码形式，最后写入到gui.ui.ixx文件里
# gui.ui.ixx文件可能存在，如果存在的话，就根据全局变量overwrite是否为true来决定是否覆写，如果不覆写的话，那么除了USER_DECLARE注释块内的其余全部刷新


import os
import re

def generate_code(file_path,data, overwrite):
    # 收集所有变量和调用信息
    variables = []
    fonts_lines = []
    img_lines = []
    code_lines = []


    for file_info in data.values():
        # 收集变量定义
        variables.extend(file_info.get('variables', []))

        # 收集资源声明
        fonts_lines = file_info['resource'].get('fonts', [] )
        fonts_lines =  [f"LV_FONT_DECLARE({item})" for item in fonts_lines] # 添加声明语句
        img_lines = file_info['resource'].get('img', [] )
        img_lines =  [f"LV_IMG_DECLARE({item})" for item in img_lines] # 添加声明语句

        # 收集链式调用和LVGL调用
        for widget_name,widget_info in file_info['calls'].items():
            chain_section = '\n\t\t\t'.join(widget_info['chain'])+';'
            lvgl_section = '\n\t\t'.join(widget_info['lvgl'])
            code_lines.append(chain_section + '\n\t\t' + lvgl_section+'\n')


    # 生成代码段落
    variables_section = '\n\t'.join(variables)
    fonts_section = '\n\t'.join(fonts_lines)
    img_section = '\n\t'.join(img_lines)
    code_section = '\n\t\t'.join(code_lines) if code_lines else '// 代码段'

    # 构造新文件内容模板
    new_content = f"""module;
#include <lvgl.h>
export module gui:ui;
export import :render;

/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/

// ---------------- 资源声明 ----------------
extern "C" {{
    // 字体声明
    {fonts_section}
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
    // 图片声明
    {img_section}
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
    // 其他
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}}

// ---------------- 组件定义 ----------------
export namespace gui::widgets::main {{
    using namespace gui::compose;
    {variables_section}
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}}

// ================== 用户空间 ==================
/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/

// ---------------- UI接口 ----------------
export namespace gui::ui {{
    using namespace gui::widgets::main;
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}}

// ---------------- 初始化逻辑 ----------------
export namespace gui {{
    void Render::screenInit() {{
        using namespace gui::widgets::main;
        
        {code_section}
        /*!USER_DECLARE_BEGIN!*/
        /*!USER_DECLARE_END!*/
    }}
    
    void Render::eventInit() {{
        using namespace gui::widgets::main;
        /*!USER_DECLARE_BEGIN!*/
        /*!USER_DECLARE_END!*/
    }}
}}

/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/
"""

    # 文件处理逻辑
    final_file_path = os.path.normpath(f"{file_path}/gui.ui.ixx")
    if not os.path.exists(final_file_path) or overwrite:
        with open(final_file_path, "w", encoding='utf-8') as f:
            f.write(new_content)
        return

    # 保留用户声明内容
    with open(final_file_path, "r", encoding='utf-8') as f:
        old_content = f.read()

    # 使用正则表达式匹配所有用户声明块
    user_block_pattern = re.compile(
        r'/\*!USER_DECLARE_BEGIN!\*/(.*?)/\*!USER_DECLARE_END!\*/',
        re.DOTALL
    )
    old_blocks = user_block_pattern.findall(old_content)

    # 替换新内容中的声明块
    def replace_blocks(match):
        return f'/*!USER_DECLARE_BEGIN!*/{old_blocks.pop(0)}/*!USER_DECLARE_END!*/' if old_blocks else match.group(0)

    updated_content = user_block_pattern.sub(replace_blocks, new_content)

    # 写入更新后的内容
    with open(final_file_path, "w", encoding='utf-8') as f:
        f.write(updated_content)

# 示例使用
if __name__ == "__main__":
    sample_data = {
        "/path/to/file1.c": {
            "variables": ["inline Button btn_press;"],
            "calls": {
                "press": {
                    "chain": ["btn_press.init(screen)"],
                    "lvgl": [
                        "lv_obj_set_size(btn_press,100,20);",
                        "lv_obj_set_text_size(btn_press,100,20);"
                    ]
                },
                "release": {
                    "chain": ["btn_release.init(screen)",'.pos(102,12)'],
                    "lvgl": [
                        "lv_obj_set_size(btn_release,100,20);",
                        "lv_obj_set_text_size(btn_release,100,20);"
                    ]
                },
            },
            'resource':{
                "fonts": [
                    "lv_font_SourceHanSerifSC_Regular_13"
                ],
                "img": [
                    "_dianzisheji_RGB565A8_83x55"
                ]
            }
        }
    }

    # 生成或更新文件（overwrite=True表示强制覆盖）
    generate_code(sample_data, overwrite=False)
