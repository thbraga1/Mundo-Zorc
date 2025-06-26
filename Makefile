# Makefile para o projeto Zorc - Recrutamento de Guerreiros

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS =

# Diretórios
SRCDIR = .
OBJDIR = obj
# BINDIR não é mais usado para o TARGET principal, ele será criado na raiz.

# Arquivos fonte
SOURCES = main.c input.c programacao_dinamica.c heuristica.c output.c
OBJECTS = $(SOURCES:%.c=$(OBJDIR)/%.o)
HEADERS = types.h input.h programacao_dinamica.h heuristica.h output.h

# Nome do executável
TARGET = tp2  # ALTERADO

# Arquivo de entrada para o teste (pode ser sobrescrito na linha de comando)
INPUT_FILE ?= teste.txt # ADICIONADO: Permite 'make test INPUT_FILE=meu_arquivo.txt'

# Regra principal
all: $(TARGET)

# Criação do executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Compilação concluída! Executável: $(TARGET)"

# Compilação dos arquivos objeto
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS) | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criação dos diretórios
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Limpeza
clean:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)      # ALTERADO: Remove o executável tp2 da raiz
	@echo "Arquivos de compilação e executável removidos."

# Limpeza parcial (apenas objetos)
clean-obj:
	rm -rf $(OBJDIR)
	@echo "Arquivos objeto removidos."

# Instalação (copia para /usr/local/bin)
install: $(TARGET)
	@echo "Instalando $(TARGET) em /usr/local/bin/"
	sudo cp $(TARGET) /usr/local/bin/$(TARGET) # ALTERADO: usa o nome $(TARGET)
	@echo "Instalação concluída!"

# Desinstalação
uninstall:
	@echo "Removendo $(TARGET) de /usr/local/bin/" # ALTERADO
	sudo rm -f /usr/local/bin/$(TARGET)        # ALTERADO
	@echo "Desinstalação concluída!"

# Execução com arquivo de teste
test: $(TARGET)
	@echo "Executando com arquivo de teste: $(INPUT_FILE)..." # ALTERADO
	@if [ -f "$(INPUT_FILE)" ]; then \
		./$(TARGET) -i $(INPUT_FILE); \
	else \
		echo "Arquivo $(INPUT_FILE) não encontrado!"; \
		echo "Crie um arquivo $(INPUT_FILE) ou especifique um com 'make test INPUT_FILE=nome_do_arquivo.txt'."; \
	fi

# Debug - compilação com informações de debug
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET)
	@echo "Versão debug compilada!"

# Release - compilação otimizada
release: CFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)
	@echo "Versão release compilada!"

# Verificação de vazamentos de memória (requer valgrind)
valgrind: $(TARGET)
	@echo "Executando verificação de memória com $(INPUT_FILE)..." # ALTERADO
	@if [ -f "$(INPUT_FILE)" ]; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET) -i $(INPUT_FILE); \
	else \
		echo "Arquivo $(INPUT_FILE) não encontrado! Execute 'make exemplo' ou especifique um arquivo."; \
	fi

# Análise estática do código (requer cppcheck)
analyze:
	@echo "Executando análise estática..."
	cppcheck --enable=all --std=c99 $(SRCDIR)/*.c

# Help - mostra as opções disponíveis
help:
	@echo "Makefile para o projeto Zorc - Recrutamento de Guerreiros"
	@echo ""
	@echo "Opções disponíveis:"
	@echo "  all        - Compila o projeto (padrão). Executável: tp2"
	@echo "  clean      - Remove todos os arquivos gerados (incluindo tp2)."
	@echo "  clean-obj  - Remove apenas os arquivos objeto."
	@echo "  debug      - Compila versão debug."
	@echo "  release    - Compila versão release otimizada."
	@echo "  install    - Instala o executável tp2 no sistema."
	@echo "  uninstall  - Remove o executável tp2 do sistema."
	@echo "  test       - Executa o programa com um arquivo de teste."
	@echo "               Use 'make test INPUT_FILE=seu_arquivo.txt' para um arquivo específico."
	@echo "               Padrão é 'teste.txt'."
	@echo "  exemplo    - Cria arquivo teste.txt com exemplo."
	@echo "  valgrind   - Executa verificação de memória com um arquivo de teste."
	@echo "  analyze    - Executa análise estática do código."
	@echo "  help       - Mostra esta ajuda."
	@echo ""
	@echo "Uso típico:"
	@echo "  make exemplo          # Cria arquivo de teste.txt"
	@echo "  make                  # Compila o projeto (gera ./tp2)"
	@echo "  make test             # Executa ./tp2 -i teste.txt"
	@echo "  make test INPUT_FILE=minha_entrada.txt # Executa ./tp2 -i minha_entrada.txt"

# Declara que estes targets não são arquivos
.PHONY: all clean clean-obj install uninstall test exemplo debug release valgrind analyze help

# Mostra variáveis (para debug do Makefile)
show-vars:
	@echo "CC = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
	@echo "SOURCES = $(SOURCES)"
	@echo "OBJECTS = $(OBJECTS)"
	@echo "TARGET = $(TARGET)"
	@echo "INPUT_FILE = $(INPUT_FILE)"