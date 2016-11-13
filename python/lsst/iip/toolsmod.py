import subprocess

def get_timestamp():
    return subprocess.check_output('date +"%Y-%m-%d %H:%M:%S.%5N"', shell=True)

def get_epoch_timestamp():
    return subprocess.check_output('date +"%s%N"', shell=True)

def singleton(object, instantiated=[]):
    assert object.__class__ not in instantiated, \
        "%s is a Singleton class but is already instantiated" % object.__class__
    instantiated.append(object.__class__)




""" Exception classes 
""" 
class L1Exception(Exception): 
    pass 

class L1Error(L1Exception): 
    """ Raise as general exception from main execution layer """
    def __init__(self, arg): 
        self.errormsg = arg
        raise

class L1MessageError(L1Exception): 
    """ Raise when asserting check_message in XML returns exception """
    def __init__(self, arg): 
        self.errormsg = arg

class L1RedisError(L1Exception):
    """ Raise when unable to connect to redis """
    def __init__(self, arg): 
        self.errormsg = arg

class L1RabbitConnectionError(L1Exception):
    """ Raise when unable to connect to rabbit """
    def __init__(self, arg): 
        self.errormsg = arg

