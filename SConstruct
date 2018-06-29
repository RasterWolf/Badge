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
                  CPPPATH   = ['/opt/vc/include/','/opt/vc/include/interface/vcos/pthreads','/opt/vc/include/interface/vmcs_host/linux','/opt/vc/src/hello_pi/libs/ilclient', '/opt/vc/src/hello_pi/libs/vgfont'],
                  LIBPATH	= ['/opt/vc/lib/'],
				  LIBS      = ['SDL2', 'SDL2_image','brcmGLESv2', 'brcmEGL', 'openmaxil' ,'bcm_host', 'vcos', 'vchiq_arm', 'pthread', 'rt', 'm'],
				  CPPDEFINES= [''])
                  

env.Program('badge', Glob('*.cpp'))
