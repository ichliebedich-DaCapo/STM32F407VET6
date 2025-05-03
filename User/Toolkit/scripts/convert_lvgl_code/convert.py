import json
import extract_info
import parser
import generate_code
import copy_files

temp_dir = r'../../../../Temp'
target_project_dir = r"../../../../Projects/RLC/ui"

if __name__ == "__main__":
    # 输入要转换的文件路径
    # r"C:/Users/fairy/Desktop/generated"
    input_dir = f"{temp_dir}"
    # 输入目标路径
    output_dir =f"{target_project_dir}"

    # ====== 提取代码信息 ======
    results = extract_info.extract(root_path=input_dir,file_pattern="temp.c")
    # ====== 解析代码信息 ======
    results = parser.parser(results)
    # ====== 生成代码 ======
    # 默认不覆写，输入输出路径
    generate_code.generate_code(file_path=output_dir,data=results, overwrite=True)

    # # ====== 复制所有字体 ======
    # copy_files.copy_files_by_pattern(source_dir=f"{root_path}/guider_fonts",target_dir=target_path,pattern="*.c")
    # # ====== 复制所有图片 ======
    # copy_files.copy_files_by_pattern(source_dir=f"{root_path}/images",target_dir=target_path,pattern="*.c")