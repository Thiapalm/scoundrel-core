# Build Instructions

Este projeto utiliza **CMake** como sistema de build.

## Pré-requisitos

- **Compilador C++**: Um compilador compatível com C++20 (GCC, Clang ou MSVC).
- **CMake**: Versão 3.16 ou superior.

- **Dependências opcionais** (somente para consumidores que precisam de SDL asset loading):
    - SDL2
    - SDL2_mixer
    - SDL2_image

## Como compilar

### Linux
Recomendamos realizar o build em uma pasta separada.

```bash
mkdir build-core
cd build-core
cmake .. -DBUILD_WITH_SDL=OFF
cmake --build .
```

### Windows
Certifique-se de ter o CMake e um compilador (como o Visual Studio) instalados.

Utilizando o Prompt de Comando do Desenvolvedor:

```cmd
mkdir build-core
cd build-core
cmake .. -G "Visual Studio 17 2022" -DBUILD_WITH_SDL=OFF
cmake --build . --config Release
```

Se um consumidor precisar de SDL asset loading, adicione `-DBUILD_WITH_SDL=ON` ao configurar o build. Isso só é necessário em `scoundrel-mobile` ou `scoundrel-tui`, não para o núcleo puro.

*Nota: Você também pode usar o CMake GUI para configurar o projeto no Windows.*

## Estrutura do projeto

- `src/`: Código fonte principal.
- `tests/`: Testes unitários (utiliza GoogleTest).
- `assets/`: Arquivos de configuração e localização.

## Notas Adicionais

- O projeto gera uma biblioteca estática (`libScoundrelCore.a` em Linux/macOS, `ScoundrelCore.lib` em Windows).
- O suporte a Android está condicionado à definição da variável `ANDROID`.
