from os import environ

VariantDir('build/src', 'src', duplicate=0)
VariantDir('build/lib', 'lib', duplicate=0)

flags = ['-O3', '-std=c++14']
#'-march=native'

env = Environment(ENV       = environ,
                  CXX       = 'clang++',
                  CPPFLAGS  = ['-Wno-unused-value'],
                  CXXFLAGS  = flags,
                  LINKFLAGS = flags,
                  CPPPATH   = [''],
                  LIBS      = ['SDL2', 'SDL2_image','GL','GLU','glut','X11','GLEW'],
		  CPPDEFINES= ['ON_PI'])
                  

env.Program('badge', Glob('*.cpp'))
