# alltomd

## 🔨 Build do Projeto

Este projeto utiliza CMake em conjunto com vcpkg para gerenciar dependências e compilação.

### 📦 Pré-requisitos

Antes de compilar, certifique-se de ter:

* CMake instalado (versão 4.0+)
* vcpkg configurado corretamente (`VCPKG_ROOT` definido)
* Compilador C++ compatível (ex: g++, MSVC, etc.)

---

### 🚀 Passos para build

Na raiz do projeto, execute:

```bash
cmake --preset vcpkg
cmake --build build --config Release
```

---

### ▶️ Executando

Após o build, o executável estará na pasta:

```bash
build/
```

Execute com:

```bash
./build/Release/alltomd
```

> No Windows:

```bash
build\Release\alltomd.exe
```

---

### 🧠 Observações

* O preset `vcpkg` já configura automaticamente:

  * Toolchain do vcpkg
  * Dependências
  * Diretório de build (`/build`)
* Não é necessário instalar dependências manualmente — o vcpkg cuida disso.

---
