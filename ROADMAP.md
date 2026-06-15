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

- [ ] **1. Janela + loop principal** — GLFW abre janela, trata input básico
- [ ] **2. Triângulo na tela** — VAO, VBO, shaders GLSL básicos
- [ ] **3. Abstração de Shader** — classe que lê, compila e usa shaders
- [ ] **4. Abstração de Mesh/Renderer** — separar dados de geometria do código de render
- [ ] **5. Câmera** — view/projection matrix, movimentação com teclado/mouse
- [ ] **6. Carregamento de texturas** — stb_image + UV mapping
- [ ] **7. Sistema de cenas/entidades** — organizar objetos no mundo
- [ ] **8. Iluminação básica** — Phong shading (ambient, diffuse, specular)

---

## Progresso

### Concluído
- Estrutura de pastas criada (`src/`, `include/engine/`, `assets/`, `build/`)
- CMakeLists.txt configurado para C++20
- Build e execução funcionando

### Em andamento
- Nada ainda — próximo passo: **Etapa 1 (Janela com GLFW)**
