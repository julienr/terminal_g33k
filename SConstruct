#
#-----------------------------------------------------------------------------
# This source file is part of Terminal_G33k
# Copyright (c) 2005 The Terminal_G33k Team
# Also see acknowledgements in Readme.txt

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
# or go to http://www.gnu.org/copyleft/gpl.txt
# -----------------------------------------------------------------------------


import glob

engine_files = glob.glob("engine/*.cc")
game_files = glob.glob("game/*.cc")

tglib_dir = ['engine/tglib','engine/tglib/math','engine/tglib/bvolume','engine/tglib/containers','engine/tglib/stl']

#tglib file list creation from tglib_dir
tglib_files = [glob.glob(dir+"/*.cc") for dir in tglib_dir]


#command-line options parsing
options = {}
options['debug'] = ARGUMENTS.get('debug',1)
options['optimize'] = ARGUMENTS.get('optimize',0)
options['profile'] = ARGUMENTS.get('profile',0)

if int(options['optimize']):
	options['debug'] = 0

#options specific flags
opt_cflags = ' -Wall '
opt_ldflags = ''
opt_libs = ''
if int(options['debug']):
	opt_cflags += '-DDEBUG -ggdb '
	opt_libs = 'mcheck'
	opt_ldflags = '-lmcheck'
if int(options['optimize']):
	opt_cflags += '-O1'
if int(options['profile']):
	opt_ldflags += '-pg'


env_eng = Environment(CPPPATH=['engine','engine/interface','game/interface'],
      CXX='g++',
			CXXFLAGS='-DTGLINUX -I/usr/include/SDL '+opt_cflags,
			LIBS=['dl','SDL','SDL_image','m','GL','GLU','tglib',opt_libs],
			LDFLAGS=opt_ldflags,
			LIBPATH='.')

env_gam = Environment(CPPPATH=['game','engine/interface','game/interface'],
      CXX='g++',
			CXXFLAGS='-DTGLINUX -Wall '+opt_cflags,
			LDFLAGS='-shared'+opt_ldflags)

env_tgl = Environment(CPPPATH=tglib_dir.extend('engine/interface'),
      CXX='g++',
			CXXFLAGS='-DTGLINUX -Wall '+opt_cflags,
			LDFLAGS='-shared'+opt_ldflags)


env_tgl.SharedLibrary(target='tglib',source=tglib_files)
env_eng.Program(target='tg',source=engine_files)
env_gam.SharedLibrary(target='game',source=game_files)
