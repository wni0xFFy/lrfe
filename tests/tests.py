from pathlib import Path
import subprocess
import os
folder = Path("tests_sources/")

files = [f for f in folder.iterdir() if f.is_file()]

passed_count = 0
failed_count = 0

for f in files:
    if '.lr' in f.name :
        result = subprocess.run(
            ["../lrfe", str(folder) + '/' + f.name],
            capture_output=True,  
            text=True,           
            encoding="utf-8"
        )
        if result.returncode != 0:
            print(f"[FAILED] {f.name} error: {result.stdout}")
            failed_count = failed_count + 1
        else:
            os.chmod(f"{str(folder) + '/' + Path(f.name).stem}", 755)
            res = subprocess.run(
                [f"{str(folder) + '/' + Path(f.name).stem}"],
                capture_output=True,  
                text=True,           
            )
            if res.returncode == 0:
                print(f"[PASSED] {f.name}")
                passed_count = passed_count + 1


files = [f for f in folder.iterdir() if f.is_file()]
for f in files:
    if '.lr' in f.name :
        pass
    else:
        os.remove(str(folder) + '/' + f.name)

print("TESTS ENDED. SUMMARY:")
print(f"[PASSED] {passed_count} tests\n[FAILED] {failed_count} tests\n")
