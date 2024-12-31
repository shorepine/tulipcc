
# This file is part of the micropython-ulab project, https://github.com/v923z/micropython-ulab
#
# The MIT License (MIT)
#
# Copyright (c) 2022 Phil Jepsen

import sys

def set_module(module):
    """Decorator for overriding __module__ on a function or class.
    Example usage::
        @set_module('numpy')
        def example():
            pass
        assert example.__module__ == 'numpy'
    """
    def decorator(func):
        if module is not None:
            sys.modules[func.__globals__['__name__']] = module
        return func
    return decorator