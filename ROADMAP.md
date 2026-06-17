# Game Engine — Roadmap

## Stack
| Propósito | Biblioteca |
|---|---|
| Janela + contexto OpenGL | GLFW |
| Carregamento de funções OpenGL | GLAD |
| Matemática (vetores, matrizes) | GLM |
| Carregamento de imagens | stb_image (header-only) |

---

## Etapas

- [x] **1. Janela + loop principal** — GLFW abre janela, trata input básico
- [x] **2. Triângulo na tela** — VAO, VBO, shaders GLSL básicos
- [x] **3. Abstração de Shader** — classe que lê, compila e usa shaders
- [x] **4. Abstração de Mesh/Renderer** — separar dados de geometria do código de render
- [ ] **5. Câmera** — view/projection matrix, movimentação com teclado/mouse
- [ ] **6. Carregamento de texturas** — stb_image + UV mapping
- [ ] **7. Sistema de cenas/entidades** — organizar objetos no mundo
- [ ] **8. Iluminação básica** — Phong shading (ambient, diffuse, specular)

---

## Progresso

### Concluído
- Estrutura de pastas criada (`src/`, `include/engine/`, `assets/`, `build/`)
- CMakeLists.txt configurado para C++20 com GLFW, GLAD e GLM
- Etapa 1: janela GLFW + contexto OpenGL 3.3 + loop principal funcionando
- Etapa 2: triângulo na tela com VAO, VBO e shaders GLSL
- Etapa 3: classe Shader lê arquivos `.glsl`, compila e linka na GPU

### Em andamento

- Etapa 4: abstração de Mesh/Renderer
