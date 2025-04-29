# 现在里面存储了各式各样的信息
#   变量类型：变量名、值和可能的变量类型，用以当成键值，匹配到【无变量类型且有lv_obj类似的函数调用】，那么就作为变量类型确定，收获【变量名】和组件类型且有
#   函数调用类型：第一个参数是必然是组件类型，以此为键值与前面的匹配到一起
#   前面两个都没匹配到的，打印信息来警告
# 组织信息：
#   根据键值把不同的函数调用组合放到字典里，{var,funcs},funcs:{func_name,['arg' 'arg' '']} 此时是info字典
#
#   变量定义转换：
#       去除ui->前缀，添加组件类型前缀，并根据[值]来确定组件类型，压入一个列表里，作为变量定义。如果找不到就用原lvgl函数
#   链式调用转换：
#       根据函数名来确定转换后的函数名，压入一个列表里，作为函数调用
#   组合信息：
#       定义一个字典，保存var（去除ui->）、变量定义和函数定义
import os
import re

import yaml
from pathlib import Path
def is_field_valid(config_dict, key_path):
    """
    检查字段存在且值不为空
    :param config_dict: 配置字典（如整个 config 或嵌套子字典）
    :param key_path: 要检查的字段名（单层）
    :return: bool (True=存在且非空)
    """
    return key_path in config_dict and config_dict[key_path] is not None

def load_config():
    with open('config.yml', encoding='utf-8') as f:  # 添加编码参数
        config = yaml.safe_load(f) or {}
    return config
cfg = load_config()


def parser(info):
    # 检查配置文件的完整性
    if not is_field_valid(cfg, 'mappings'):
        ValueError("配置文件缺少 'mappings' 字段")
    cfg_mappings = cfg['mappings']

    if not is_field_valid(cfg_mappings, 'variable_types') or not is_field_valid(cfg_mappings, 'function_calls'):
        ValueError("配置文件缺少 'variables' 或 'calls' 字段")
    cfg_variables = cfg_mappings['variable_types']
    cfg_calls = cfg_mappings['function_calls']

    process_results ={}
    for file_path, data in info.items():
        print(f"\r\n处理文件: {os.path.basename(file_path)}")
        variables = []
        calls = []
        parent_name = '' # 父组件名称

        # 组件定义
        for var in data['variables']:
            var_type = ''
            func_name  = '' # 组件创建的函数名
            merge_name = '' # 变量的合成名

            # 获取组件创建的函数名和父组件名
            pattern = r"(\w+)\(\s*.*?ui->(\w+)"
            match = re.search(pattern, var['value'])
            if match:
                func_name = match.group(1)
                parent_name = match.group(2)
                # 获得合成名的前缀
                pattern  = r'lv_(\w+)_create'
                match = re.search(pattern, func_name)
                if match:
                    prefix = match.group(1)
                    var_name = var['name']
                    merge_name = f'{prefix}_{var_name}'
            else:
                # 变量赋值出现了不该出现的，比如定义样式
                ValueError(f"无法解析变量定义: {var['value']}")

            # 确定变量的组件类型
            if is_field_valid(cfg_variables, func_name):
                var_type = cfg_variables[func_name]
            else:
                var_type = 'Component'

            # 添加变量定义
            variables.append(f'inline {var_type} {merge_name};')


        # 函数调用
        for call in data['calls']:
            print(f"    {call['name']}    {', '.join(call['params'])}")


        process_results[file_path] ={
            'variables':variables,
            'calls':calls
        }

    return process_results

# 使用示例
if __name__ == "__main__":
    results = {
        "/path/to/file1.c":{
            "calls": [
                {
                    'name': 'lv_btn_create',
                    'params': ['ui->screen', '100'],
                    'source_file': '/path/to/file.c'
                }
            ],
            "variables": [
                {
                    'name': 'press',
                    'value': 'lv_btn_create(ui->screen)',
                    'var_type': 'lv_obj_t*',
                    'source_file': '/path/to/file.c'
                }
            ]
        },
    }

    parser(results)

