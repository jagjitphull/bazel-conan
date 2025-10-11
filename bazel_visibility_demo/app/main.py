from lib import math_lib
from tools import helper

# from internal import internal_tool                # ❌ Uncommenting this should break

print("2 + 3 =", math_lib.add(2, 3))
helper.print_message("Main executed")

# internal_tool._internal_use_only()                # ❌ Access not allowed
