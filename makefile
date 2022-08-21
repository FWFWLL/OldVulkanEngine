TARGET := Vulkan_C++

VKVERSION := 1.3.211.0

VKINCL := -IC:/VulkanSDK/$(VKVERSION)/Include
VKLIB := -LC:/VulkanSDK/$(VKVERSION)/Lib -lvulkan-1

CXX := g++
CPPFLAGS := -Iinclude -MMD -MP $(VKINCL)
CXXFLAGS := -std=c++17 -m64 -O0 -static-libgcc -static-libstdc++ -Wall -Wextra
LDFLAGS := -Llib -lglfw3 $(VKLIB)

BINDIR := bin
SRCDIR := src
OBJDIR := obj

OBJDIRS = $(OBJDIR)/ $(subst src,$(OBJDIR), $(wildcard $(SRCDIR)/**/))
SRC = $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

.PHONY: all clean run shaders

all: $(TARGET) shaders

clean:
	@rm -rv $(BINDIR)/$(TARGET) $(OBJDIR) shaders/*.spv

run: all
	@$(BINDIR)/$(TARGET)

$(TARGET): $(OBJ) | $(BINDIR)
	$(CXX) $^ $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $(BINDIR)/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIRS)
	$(CXX) -c $< $(CPPFLAGS) $(CXXFLAGS) -o $@

$(BINDIR) $(OBJDIRS):
	mkdir -p $@

-include $(OBJ:.o=.d)

# Vulkan SPIR-V compiled shader make targets

SPVC := C:/VulkanSDK/$(VKVERSION)/Bin/glslc.exe

shaders: shaders/shader.vert.spv shaders/shader.frag.spv

shaders/shader.vert.spv: shaders/shader.vert
	$(SPVC) $^ -o $@

shaders/shader.frag.spv: shaders/shader.frag
	$(SPVC) $^ -o $@
