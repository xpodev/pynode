from asyncio import sleep as asleep, get_event_loop
from time import sleep
from concurrent.futures import ThreadPoolExecutor
from functools import partial, wraps

class to_async:
    def __init__(self, *, executor=None):
        self.executor =  executor
    
    def __call__(self, blocking):
        @wraps(blocking)
        async def wrapper(*args, **kwargs):
            loop = get_event_loop()
            if not self.executor:
                self.executor = ThreadPoolExecutor()

            func = partial(blocking, *args, **kwargs)
        
            return loop.run_in_executor(self.executor, func)

        return wrapper


@to_async()
def wait_sync(time: float):
    sleep(time)


async def concurrent(*items):
    for i in items:
        print(f"concurrent: {i}")
        await asleep(0.1)


async def main():
    print("main: 0")
    await asleep(1)
    print("main: 1")
    # await wait_sync(1)
    print("main: 2")
    # wait_sync(1)
    print("main: 3")
    concurrent(*[i for i in range(100)])
    await asleep(11)
    print("main: 4")


main()
