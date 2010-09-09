#
# Android Zip Signing Tool
# NathanForbes
#

import os
import sys
import subprocess

tools = os.path.join(os.getcwd(), 'tools')
keys = ['signapk.jar',
        'testkey.pk8',
        'testkey.x509.pem']

def exist_sig_tools():
    if not os.path.isdir(tools):
        print 'Error: %s directory not found' % tools
        return False
    else:
        for k in keys:
            if not os.path.isfile(os.path.join(tools, k)):
                print 'Error: %s not found' % k
                return False
    return True

class SigningError(Exception):
    pass

def sign_package(z):
    if not exist_sig_tools():
        sys.exit(1)
    try:
        os.makedirs(os.path.join(os.getcwd(), 'signed'))
    except:
        pass
    signed_zip = os.path.join(os.getcwd(), 'signed',
                            os.path.basename(z).replace('.zip', '-signed.zip'))
    print "Signing package: %s" % os.path.basename(z)
    sign = ['java',
            '-Xmx512m',
            '-jar', os.path.join(tools, keys[0]),
            '-w', os.path.join(tools, keys[2]), os.path.join(tools, keys[1]),
            z, signed_zip]
    s = subprocess.Popen(sign)
    s.communicate()
    if s.returncode != 0:
        raise SigningError('Signing %s failed' % z)
    if os.path.isfile(signed_zip):
        print "Success, signed zip located at %s" % signed_zip

def help():
    print "Usage: %s [<full/path/to/zip>]" % os.path.basename(sys.argv[0])
    print "*** Must be executed from directory containing 'tools' ***"

if __name__ == '__main__':
    if len(sys.argv) > 1:
        if sys.argv[1] == '--help':
            help()
            sys.exit()
        for ZIP in sys.argv[1:]:
            if not os.path.isfile(os.path.expanduser(ZIP)):
                print 'Error: %s not found' % ZIP
                sys.exit(2)
            if not ZIP.endswith('.zip'):
                print 'Error: %s not a zipfile' % ZIP
                sys.exit(2)
            sign_package(os.path.expanduser(ZIP))
    else:
        help()
        sys.exit(1)