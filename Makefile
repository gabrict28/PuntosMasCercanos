#Nombre del archivo ejecutable final
TARGET = par_puntos

#Compilador C++
CXX = g++

#Flags de compilación
CXXFLAGS = -Wall -std=c++17

#Archivos fuente
SRCS = main.cpp

# Archivos objeto
OBJS = $(SRCS:.cpp=.o)

# Regla principal
all: $(TARGET)

#Cómo construir el ejecutable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

#Cómo compilar los .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Limpiar archivos intermedios
clean:
	rm -f *.o

#Limpiar todo (incluido el ejecutable)
cleanall: clean
	rm -f $(TARGET)

