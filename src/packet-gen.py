from ParseDefinition  import parseDefinition  as ParseDefinition
from MakeClass        import makeClass        as MakeClass
from ServerDispatcher import serverDispatcher as ServerDispatcher
from ClientDispatcher import clientDispatcher as ClientDispatcher

import sys

if len(sys.argv) != 4:
    print("Usage: python packet-gen.py <input file name> <client output file name> <server output file name>")
    exit(1)

args = sys.argv[1:]
[inputfilename, clientfilename, serverfilename] = args[0:3]

f = open(inputfilename)
lines = f.readlines() + ['\n']

types = ParseDefinition(lines)
for i in range(len(types)):
    types[i]['index'] = i + 1

def isClientToServerOnly(T):
    return 'C2S' in T['name'] and not 'S2C' in T['name']

def isServerToClientOnly(T):
    return 'S2C' in T['name'] and not 'C2S' in T['name']

def isBoth(T):
    return not 'S2C' in T['name'] and not 'C2S' in T['name']

def MakeReadOnlyClass(T):
    return MakeClass(T, True, False)

def MakeWriteOnlyClass(T):
    return MakeClass(T, False, True)

def MakeReadWriteClass(T):
    return MakeClass(T, True, True)

ClientClasses = [
        MakeReadOnlyClass (T) for T in types if isServerToClientOnly(T)
    ] + [
        MakeWriteOnlyClass(T) for T in types if isClientToServerOnly(T)
    ] + [
        MakeReadWriteClass(T) for T in types if isBoth(T)
    ]

ServerClasses = [
        MakeReadOnlyClass (T) for T in types if isClientToServerOnly(T)
    ] + [
        MakeWriteOnlyClass(T) for T in types if isServerToClientOnly(T)
    ] + [
        MakeReadWriteClass(T) for T in types if isBoth(T)
    ]

disclaimer = '''

 /*************************************************************\ 
 \                                                             / 
 /   THIS FILE WAS AUTOMATICALLY GENERATED USING PACKET-GEN    \ 
 \                                                             / 
 /    DO NOT MODIFY THIS FILE, SEE PACKET-GEN INSTRUCTIONS     \ 
 \            FOR UPDATING THE PACKET DEFINITIONS              / 
 /                                                             \ 
 \*************************************************************/ 


'''
includes = '#pragma once\n#include <vector>\n#include <fstream>\n#include <string>\n#include <functional>\n\n'
namespace = lambda ns: f'namespace {ns} ' + '{\n'

with open(serverfilename, 'w+') as f:
    f.write(disclaimer + includes + namespace("NS_Server"))
    for C in ServerClasses:
        f.write(C)
        f.write('\n')
    f.write(ServerDispatcher([T for T in types if isClientToServerOnly(T) or isBoth(T)]) + "};\n")

with open(clientfilename, 'w+') as f:
    f.write(disclaimer + includes + namespace("NS_Client"))
    for C in ClientClasses:
        f.write(C)
        f.write('\n')
    f.write(ClientDispatcher([T for T in types if isServerToClientOnly(T) or isBoth(T)]) + "};\n")
