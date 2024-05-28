# VC
TP Visão por Computador

MACOS- terminal
// Project 8

 /usr/bin/clang++ -std=c++11 -fcolor-diagnostics -fansi-escape-codes -g \
-I/opt/homebrew/Cellar/opencv/4.9.0_8/include/opencv4 \
-L/opt/homebrew/Cellar/opencv/4.9.0_8/lib \
-lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio \
project8.cpp \
-o project8

 ./project8

---------------------

// Project 7

 /usr/bin/clang++ -std=c++11 -fcolor-diagnostics -fansi-escape-codes -g \
-I/opt/homebrew/Cellar/opencv/4.9.0_8/include/opencv4 \
-L/opt/homebrew/Cellar/opencv/4.9.0_8/lib \
-lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio \
project7.cpp \
-o project7

./project7

Windows- cmd

g++ -std=c++11 -g -I"C:\opencv\build\include" -L"C:\opencv\build\x64\mingw\lib" -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio project7.cpp -o project7.exe

project7.exe


---------------------

Explicação dos parâmetros:
-std=c++11: Define o padrão C++11.
-g: Inclui informações de depuração.
-I"C:\opencv\build\include": Define o caminho para os arquivos de cabeçalho do OpenCV.
-L"C:\opencv\build\x64\mingw\lib": Define o caminho para as bibliotecas do OpenCV.
-lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio: Liga contra as bibliotecas do OpenCV necessárias.
project2.cpp: O arquivo de origem C++ que você deseja compilar.
-o project2.exe: Define o nome do arquivo executável resultante.
project2.exe: Executa o arquivo compilado.


Passos para configuração no Windows:

Instale MinGW:

Baixe e instale MinGW.
Adicione o caminho para mingw/bin ao PATH do sistema.

Instale OpenCV:

Baixe o build pré-compilado do OpenCV para Windows.
Extraia o conteúdo em C:\opencv ou outra localização de sua escolha.

Configure o Ambiente:

Certifique-se de que os caminhos para C:\opencv\build\include e C:\opencv\build\x64\mingw\lib estão corretos.

Compile e Execute:

Abra o CMD, navegue até o diretório do seu projeto e execute o comando de compilação acima.
Certifique-se de que os caminhos fornecidos no comando -I e -L correspondem à localização correta dos arquivos de cabeçalho e bibliotecas do OpenCV no seu sistema.

---------------------

Para baixar e instalar o OpenCV no Windows via linha de comando (cmd), você pode usar o utilitário winget (Windows Package Manager) ou o choco (Chocolatey). Aqui estão os passos para ambos os métodos:

Método 1: Usando winget
Abrir o CMD como Administrador:

Pressione Win + X e selecione "Windows Terminal (Admin)" ou "Prompt de Comando (Admin)".
Instalar OpenCV com winget:

cmd
Copiar código
winget install -e --id OpenCV.OpenCV

Método 2: Usando choco
Instalar Chocolatey:
Se você ainda não tem o Chocolatey instalado, execute o seguinte comando no CMD:

cmd
Copiar código
@powershell -NoProfile -ExecutionPolicy Bypass -Command "iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))" && SET "PATH=%PATH%;%ALLUSERSPROFILE%\chocolatey\bin"

Instalar OpenCV com choco:
cmd
Copiar código
choco install opencv

Verificando a Instalação:
Após a instalação, verifique se o OpenCV foi instalado corretamente. O OpenCV deve estar instalado em um diretório acessível, como C:\tools\opencv ou outro conforme especificado durante a instalação.

Configurando o Ambiente:
Adicionar o Caminho do OpenCV ao PATH do Sistema:

Abra o CMD como Administrador.
Execute o seguinte comando para adicionar o caminho do OpenCV ao PATH (substitua pelo caminho correto se necessário):

cmd
Copiar código
setx -m OPENCV_DIR "C:\tools\opencv\build"
Adicionar Bibliotecas e Incluídos:

Certifique-se de que os caminhos para as bibliotecas e arquivos de cabeçalho do OpenCV estão configurados corretamente no seu compilador.


