import os
import re

# 1. Caminhos dos diretórios
DIRETORIO_SCRIPT = os.path.dirname(os.path.abspath(__file__))
DIRETORIO_PAI = os.path.abspath(os.path.join(DIRETORIO_SCRIPT, ".."))

# 2. Mapeamento de arquivos
arquivos = {
    os.path.join(DIRETORIO_PAI, "main.html"): "APLICACAO",
}

codigo_h = "// Arquivo gerado automaticamente!\n#ifndef SITES_H\n#define SITES_H\n\n"

def minificar_codigo(texto):
    """Remove excessos sem quebrar o HTML, CSS ou JS."""
    # Remove comentários HTML ()
    texto = re.sub(r'', '', texto, flags=re.DOTALL)
    
    # Remove comentários CSS e JS (/* ... */)
    texto = re.sub(r'/\*.*?\*/', '', texto, flags=re.DOTALL)
    
    # Troca múltiplas quebras de linha/espaços por um único espaço
    texto = re.sub(r'\s+', ' ', texto)
    
    # Remove espaços entre tags do HTML (maior ganho de espaço)
    texto = re.sub(r'>\s+<', '><', texto)
    
    return texto.strip()

for caminho_arquivo, nome_variavel in arquivos.items():
    try:
        with open(caminho_arquivo, "r", encoding="utf-8") as f:
            conteudo = f.read()
            
            # 1. Esmaga o código para economizar espaço
            conteudo_min = minificar_codigo(conteudo)
            
            # 2. Divide em blocos ANTES de aplicar o escape (Isso evita o erro de compilação)
            tamanho_bloco = 120
            blocos = [conteudo_min[i:i+tamanho_bloco] for i in range(0, len(conteudo_min), tamanho_bloco)]
            
            # 3. Agora aplica o escape (\n, \", \\) em cada bloco de forma segura
            blocos_escapados = [b.replace('\\', '\\\\').replace('"', '\\"') for b in blocos]
            
            # 4. Une os blocos com o padrão do C para strings multilinha
            string_c_final = '"\n"'.join(blocos_escapados)
            
            codigo_h += f'const char* {nome_variavel} = "{string_c_final}";\n\n'
            
    except FileNotFoundError:
        print(f"Aviso: Arquivo não encontrado -> {caminho_arquivo}")

codigo_h += "#endif\n"

# 5. Salva o arquivo final
caminho_saida = os.path.join(DIRETORIO_PAI, "paginasHTTP.h")

with open(caminho_saida, "w", encoding="utf-8") as f:
    f.write(codigo_h)

print(f"Arquivo gerado com sucesso! A compilação no C deve ocorrer perfeitamente agora.\n{caminho_saida}")