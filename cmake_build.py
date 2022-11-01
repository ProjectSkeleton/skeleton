import platform
import os

if platform.system() == "Windows":
  os.system("cmake -B build")
elif platform.system() == "Darwin":
  os.system("cmake -B build -G Xcode")
else:
  print("Operating system not yet supported")
