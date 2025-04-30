import json
import extract_info
import parser
import generate_code
import copy_files

if __name__ == "__main__":
    # 输入要转换的文件路径
    root_path = r"C:\Users\fairy\Desktop\generated"
    results = extract_info.extract(
        root_path=root_path,
        file_pattern="setup_scr_*.c"
    )
    results = parser.parser(results)
    # 默认不覆写，输入输出路径
    generate_code.generate_code(file_path='.',data=results, overwrite=False)

    # 复制所有字体
    copy_files.copy_files_by_pattern(source_dir=f"{root_path}\guider_fonts",
                          target_dir="../../../Projects/SignalDistortionMeasurement/ui",
                          pattern="*.c")
    # 复制所有图片
    copy_files.copy_files_by_pattern(source_dir=f"{root_path}\images",
                          target_dir="../../../Projects/SignalDistortionMeasurement/ui",
                          pattern="*.c")