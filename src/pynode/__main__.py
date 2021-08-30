import os, sys, runpy
try:
    file = sys.argv[1]
except IndexError:
    print(f"{sys.argv} must get a file path")
    exit(1)

os.environ["PYNODE_SET_HANDLER"] = "1"
runpy.run_path(os.path.join(os.getcwd(), sys.argv[1]), run_name="__main__")
