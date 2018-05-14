# Compiler to use. If using MPI then say mpicxx here (or mpic++)
# and also say -DUSE_MPI below
CXX = g++

# We're using ansi C++
CPPFLAGS += -pedantic -std=c++98

# Enable MPI (you must set the proper
# compiler wrapper under CXX above)
# CPPFLAGS += -DUSE_MPI

# Enable warnings and treat warnings as errors
CPPFLAGS += -Wall -Wendif-labels

# Treat warnings as errors
# (hdf5 on Ubuntu does not pass this, so it's
# commented out by default now)
#CPPFLAGS += -Werror

# This enables additional debugging
#CPPFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_PROFILE

#When using IoNg one has to compile with HDF5 libraries
CPPFLAGS += -DUSE_IO_NG
CPPFLAGS += -I/usr/include/hdf5/serial
LDFLAGS += -lhdf5_hl_cpp -lhdf5_cpp -lhdf5_hl -lhdf5

# This makes the code use long instead of short integers
#CPPFLAGS +=-DUSE_LONG

# This makes the code use float instead of double
#CPPFLAGS += -DUSE_FLOAT

# This enables signals
#CPPFLAGS +=-DUSE_SIGNALS

# This enables gsl support
#CPPFLAGS +=-DUSE_GSL
#LDFLAGS += -lgsl -lgslcblas

# This enables the custom allocator (use only for debugging)
#CPPFLAGS += -DUSE_CUSTOM_ALLOCATOR

#CPPFLAGS += -DUSE_BOOST

# Specify the strip command to use (or use true to disable)
STRIP_COMMAND = true

