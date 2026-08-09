# Gravitas Style Guidelines

---

## 1. Naming Conventions

| Entity | Style | Example | Rule / Rationale |
| :--- | :--- | :--- | :--- |
| **Types / Classes / Structs** | `PascalCase` | `BodySystem`, `QuadTree` | Clear, standard type identifiers. |
| **Functions / Methods** | `PascalCase` | `UpdatePhysics()`, `GetMass()` | Instantly distinguishes calls from variables. |
| **Local Variables** | `snake_case` | `body_count`, `delta_time` | Clean, low visual noise in internal logic. |
| **Function Parameters** | `camelCase` | `deltaTime`, `particleId` | Distinguishes inputs from local variables. |
| **Member Variables** | `m_camelCase` | `m_position`, `m_bodyCount` | Eliminates ambiguity between class state and locals. |
| **Static Member Variables** | `s_camelCase` | `s_instanceCount` | Explicit lifecycle and scope awareness. |
| **Global Variables** | `g_camelCase` | `g_applicationContext` | Visually flags global state (use sparingly). |
| **Constants / Enums** | `kPascalCase`, `PascalCase` | `kMaxBodies`, `State::Running` | Avoid `ALL_CAPS` macros; scoped enums follow types. |
| **Namespaces** | `lowercase` | `gravitas::physics` | Short, flat, no underscores. |

---

## 2. File & Directory Structure

* **Headers & Implementations:** Standardize on `.hpp` for headers and `.cpp` for implementations.
* **Class Mapping:** File names must match the primary class name exactly (`BodySystem.hpp` / `BodySystem.cpp`).
* **Header Guard:** Use `#pragma once` at the very top of every header file.
* **Include Formatting:**
  * System/Library headers first: `#include <vector>`, `#include <glm/vec3.hpp>`
  * Engine headers second: `#include "Core/Logger.hpp"`

---

## 3. Architecture & Code Decisions

### A. Memory & Ownership
* **Raw Pointers (`T*`):** Used *only* for non-owning references/observing. **Never call `delete` manually.**
* **Smart Pointers (`std::unique_ptr`):** Default for exclusive ownership.
* **Shared Pointers (`std::shared_ptr`):** Avoid unless true shared-lifetime semantics are legally required.
* **Value Semantics First:** If a system can live directly on the stack or inside a contiguous `std::vector`, keep it value-based!

### B. Function Parameters & Return Types
* **Primitive types (`int`, `float`, `bool`):** Pass by value (`float deltaTime`).
* **Complex Read-Only objects:** Pass by `const&` (`const glm::vec3& position`).
* **Out-Parameters / Modifiers:** Pass by non-const reference or return a custom struct via C++20 structured bindings.

### C. Const-Correctness & Initialization
* Mark every member method `const` if it does not modify the object state (`float GetMass() const;`).
* Provide default member initializers inside header class declarations (`float m_mass = 1.0f;`).
* Prefer uniform initialization (`size_t count{0};`).

### D. Error Handling & Performance
* **No Engine-Loop Exceptions:** Real-time simulation steps must not throw or catch exceptions.
* **Invariants:** Use `assert()` to enforce internal state logic during Debug builds.
* **Expected Results:** Use `std::optional<T>` or `std::expected<T, ErrorCode>` for operations that can predictably fail (e.g., asset loading, file I/O).

---
