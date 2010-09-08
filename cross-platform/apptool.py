#
# Backup/Restore APK's to & from your desktop
# nathanForbes
#
# Prereqs:
# - Python interpreter executable located in your system's PATH
# - ADB executable located in PATH
#
# Usage:
#       python apptool.py [OPTION]
#

import os
import pwd
import sys
import shutil
import zipfile
import optparse
import subprocess

class ExternalError(RuntimeError):
    pass

def command(args, v=True, **kwargs):
    if v:
        print "Running: ", " ".join(args)
    return subprocess.Popen(args, **kwargs)

home = os.path.expanduser('~')
bdir = os.path.join(home, 'app_backup')
zipname = "%s-app_backup" % (pwd.getpwuid(os.getuid()).pw_name)
done_zip = zipname + '.zip'

def backup():
    try:
        if os.path.isdir(bdir):
            shutil.rmtree(bdir)

        os.makedirs(os.path.join(bdir, 'APPS', 'app'))
        os.makedirs(os.path.join(bdir, 'APPS', 'app-private'))

        cmd = ["adb", "pull", "/data/app", os.path.join(bdir, 'APPS', 'app')]
        run = command(cmd, stdout=subprocess.PIPE)
        run.communicate()
        if run.returncode != 0:
            raise ExternalError("Error: ADB pull fail")

        cmd = ["adb",
               "pull",
               "/data/app-private",
               os.path.join(bdir,
                            'APPS',
                            'app-private')]
        run = command(cmd, stdout=subprocess.PIPE)
        run.communicate()
        if run.returncode != 0:
            raise ExternalError("Error: ADB pull fail")

        os.chdir(bdir)

        print "Zipping up all backed up apks..."
        z = zipfile.ZipFile(done_zip, 'w', zipfile.ZIP_DEFLATED)
        for dirpath, dirnames, filenames in os.walk('APPS'):
            for filename in filenames:
                abspath = os.path.join(dirpath, filename)
                arcpath = abspath[len('APPS')+1:]
                z.write(abspath, arcpath)
        z.close()

        sep = os.linesep
        applist = sep.join(os.listdir(os.path.join('APPS', 'app')))
        privlist = sep.join(os.listdir(os.path.join('APPS', 'app-private')))

        print "Generating info text..."
        f = open("backup_info.txt", mode="w")
        f.write("Thanx for using AppTool!"+sep+sep+"="*60+sep+sep+
                "Apps:"+sep+applist+sep+sep+"Private apps:"+sep+
                privlist+sep+sep+"="*60+sep+"All backed up apps "
                "located in "+os.path.join(bdir, done_zip)+sep+sep)
        f.close()

        shutil.rmtree("APPS")
        print "Done backing up apps. Open %s for details" \
               % os.path.join(bdir, 'backup_info.txt')
        sys.exit()
    except KeyboardInterrupt:
        print "\nAborting..."
        if os.path.isdir(bdir):
            shutil.rmtree(bdir)

def restore():
    try:
        if not os.path.isfile(os.path.join(bdir, done_zip)):
            print "Error: %s does not exist. Did you run the backup option?" \
                   % done_zip
            sys.exit(1)
        else:
            os.chdir(bdir)

            if os.path.isdir("APPS"):
                shutil.rmtree('APPS')

            ex = zipfile.ZipFile(done_zip, 'r')
            ex.extractall()
            ex.close()

            if os.path.isdir(os.path.join(bdir, 'app')):
                for apk in os.listdir(os.path.join(bdir, 'app')):
                    if apk is not None:
                        apkname = os.path.basename(apk)
                        cmd = ["adb",
                               "install",
                               os.path.join(bdir, 'app', apkname)]
                        run = command(cmd)
                        run.communicate()

            if os.path.isdir(os.path.join(bdir, 'app-private')):
                for apk in os.listdir(os.path.join(bdir, 'app-private')):
                    if apk is not None:
                        apkname = os.path.basename(apk)
                        cmd = ["adb",
                               "install",
                               os.path.join(bdir, 'app-private', apkname)]
                        run = command(cmd)
                        run.communicate()

            os.chdir(bdir)

            if os.path.isdir("app"):
                shutil.rmtree('app')
            if os.path.isdir('app-private'):
                shutil.rmtree('app-private')

            sys.exit()
    except KeyboardInterrupt:
        print "\nAborting..."
        if os.path.isdir(os.path.join(bdir, 'app')):
            shutil.rmtree(os.path.join(bdir, 'app'))
        if os.path.isdir(os.path.join(bdir, 'app-private')):
            shutil.rmtree(os.path.join(bdir, 'app-private'))

def adb_check():
    adb = command(['adb'], v=False, stderr=subprocess.PIPE)
    adb.communicate()
    if adb.returncode != 1:
        return False
    else:
        return True

def devices():
    c = ["adb", "devices"]
    dev = command(c, v=False, stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    x = list(dev.communicate())
    if len(x[0]) < 30:
        return False
    else:
        return True

def main():
    p = optparse.OptionParser(usage='usage: python %prog [OPTION]')
    p.add_option('-b', '--backup', dest='backup_apps', action='store_true',
                             help='Backup applications')
    p.add_option('-r', '--restore', dest='restore_apps', action='store_true',
                             help='Restore applications')
    opts, args = p.parse_args()

    if opts.backup_apps and opts.restore_apps:
        p.print_help()
        sys.exit(1)
    if adb_check():
        if devices():
            if opts.backup_apps:
                return backup()
            elif opts.restore_apps:
                return restore()
            else:
                p.print_help()
                sys.exit(1)
        else:
            raise ExternalError("Device not plugged in")
    else:
        raise ExternalError('ADB was not detected in your systems PATH')

if __name__ == '__main__':
    main()
