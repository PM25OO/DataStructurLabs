# ========== Makefile ==========

# 编译器
CC = gcc

# 源文件列表（当前目录下所有 .c）
SRC := $(wildcard *.c)

# 输出目录
OUT_DIR := build

# 对应的目标文件（把每个 xxx.c 替换成 build/xxx）
TARGETS := $(patsubst %.c,$(OUT_DIR)/%,$(SRC))

# 默认目标
all: $(OUT_DIR) $(TARGETS)

# 规则：把每个 .c 文件编译成 build/同名可执行文件
$(OUT_DIR)/%: %.c
	@echo "🔧 正在编译: $< → $@"
	$(CC) $< -o $@

# 如果 build/ 不存在则创建
$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

# 清理命令
clean:
	@echo "🧹 正在清理..."
	@rm -rf $(OUT_DIR)

.PHONY: all clean
# ===============================
