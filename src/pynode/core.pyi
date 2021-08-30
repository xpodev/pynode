from typing import Coroutine, Callable, Optional

def get_coroutine_handler() -> Optional[Callable[[Coroutine], None]]: 
    """Get the current handler for coroutines that are about to get deleted
    """

def set_coroutine_handler(callback: Optional[Callable[[Coroutine], None]]) -> None:
    """Get the handler for coroutines that are about to get deleted. 

    If set to None, the default handler will be used. The default handler will schedule
    the coroutine as a task, creatig a new async loop if no loop exists already.
    """

__all__ = [
    "get_coroutine_handler",
    "set_coroutine_handler"
]
