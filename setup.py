from distutils.core import setup, Extension

pynode_module = Extension('pynode.core',
                    sources = ['src/pynode/core/pynode_core.cpp'])

setup (name = 'PyNode',
       version = '1.1.0',
       description = 'PyNode allowes you to run asynchronous tasks cuncurrently.',
       author="Xpo Development",
       author_email="biny.xpo.dev",
       packages=["pynode"],
       package_dir={
              "pynode": "src/pynode"
       },
       package_data={
              "pynode": ["core.pyi"]
       },
       ext_modules = [pynode_module])
