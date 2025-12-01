# ==== Toolchain ====
CXX      := clang
CXXFLAGS := -g -Wall -Werror -Wextra
GTK_CFLAGS := $(shell pkg-config --cflags libcjson gtk4 libcurl libjwt)
GTK_LIBS   := $(shell pkg-config --libs gtk4 libcurl libjwt) -lcjson -lm -ljwt

# ==== Project structure ====
SRCDIR   := srcs
INCDIR   := -Iincludes/
OBJDIR   := .objs
TARGET   := cli

# ==== Source files ====
SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))
DEPFILES := $(OBJFILES:.o=.d)

# ==== Build rules ====
.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJFILES)
	@echo "Linking $@..."
	$(CXX) $^ -o $@ $(GTK_LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(GTK_CFLAGS) $(INCDIR) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# ==== Include auto-generated dependency files ====
-include $(DEPFILES)

# ==== Cleaning ====
.PHONY: clean fclean re

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJDIR)

fclean: clean
	@echo "Removing binary $(TARGET)..."
	@rm -f $(TARGET)

re: fclean all

