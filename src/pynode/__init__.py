from . import core
from .core import *


__all__ = [
    "get_coroutine_handler",
    "set_coroutine_handler"
]

if __name__ != '__main__':
    import os, asyncio

    try:
        if os.environ["PYNODE_SET_HANDLER"] == "1":
            set_coroutine_handler(None)
    except KeyError:
        ...

    del asyncio
    del os
