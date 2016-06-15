
from subprocess import Popen, PIPE, STDOUT

print "start"
#p = Popen(['build/LogCabin', '--config', 'logcabin-3.conf'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
#p = Popen(['build/Examples/Ops', 'mkdir', 'dir1/subdir1'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
p = Popen(['build/Examples/Ops', 'dump'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
#p = Popen(['build/Examples/Ops', 'write', 'dir1/subdir1/file1'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)
print "pid?:%s" % p.pid
#p.terminate()
#print "pid?:%s" % p.pid
grep_stdout = p.communicate(input=b'file-content is very much\n')[0]
print(grep_stdout.decode())

"""
cd Applications/coherent/logcabin
build/LogCabin --config logcabin-1.conf
build/LogCabin --config logcabin-1.conf --bootstrap
"""