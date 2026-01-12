#include "game/PathFinder.hpp"
#include "game/Field.hpp"
#include <queue>
#include <cmath>   // Para std::abs
#include <array>   // Para std::array
#include <limits>  // Para numeric_limits::max()

namespace Game
{
    // Estructura interna para representar un nodo en el algoritmo A*
    struct Node
    {
        int x, y;
        int gCost; // Costo desde el inicio (pasos dados)
        int hCost; // Heurística (estimación hasta la meta)

        // Costo Total (F = G + H)
        int fCost() const { return gCost + hCost; }

        // Sobrecarga del operador > para que la Priority Queue ordene de MENOR a MAYOR costo
        // (En C++, priority_queue saca el elemento "mayor", así que invertimos la lógica)
        bool operator>(const Node& other) const
        {
            return fCost() > other.fCost();
        }
    };

    bool PathFinder::doesPathExist(const Board& board, int startX, int startY, int targetRow)
    {
        // 1. Validaciones de seguridad
        if (!board.isValid(startX, startY)) return false;
        
        // Si ya estamos en la fila meta, el camino existe (distancia 0)
        if (startY == targetRow) return true;

        // 2. Configuración de A*
        
        // Cola de Prioridad: Siempre nos dará el nodo con menor costo F (el más prometedor)
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openSet;

        // Mapa de costos mínimos: Guardamos el mejor costo G encontrado para cada casilla
        // Usamos un array plano de 81 elementos (9x9) para máxima velocidad
        std::array<int, Board::SIZE * Board::SIZE> minCosts;
        minCosts.fill(std::numeric_limits<int>::max()); // Inicializamos a infinito

        // 3. Nodo Inicial
        int startIdx = startY * Board::SIZE + startX;
        minCosts[startIdx] = 0;

        // Heurística: Distancia vertical hacia la meta
        int hInitial = std::abs(startY - targetRow);
        openSet.push({startX, startY, 0, hInitial});

        // 4. Bucle Principal
        while (!openSet.empty())
        {
            Node current = openSet.top();
            openSet.pop();

            // A. Verificación de Meta
            if (current.y == targetRow)
            {
                return true; // ¡Llegamos!
            }

            // B. Optimización: Si sacamos un nodo que ya fue superado por un camino mejor, lo ignoramos
            int currentIdx = current.y * Board::SIZE + current.x;
            if (current.gCost > minCosts[currentIdx]) continue;

            // C. Explorar Vecinos
            // Usamos el orden del enum Direction: Up(0), Right(1), Down(2), Left(3)
            const int dx[] = {0, 1, 0, -1};
            const int dy[] = {-1, 0, 1, 0};

            // Referencia a la casilla actual para consultar sus paredes
            const Field& currentField = board.getField(current.x, current.y);

            for (int i = 0; i < 4; ++i)
            {
                Direction dir = static_cast<Direction>(i);

                // Paso 1: ¿Existe conexión física? (No hay pared bloqueando)
                if (currentField.hasPath(dir))
                {
                    int nextX = current.x + dx[i];
                    int nextY = current.y + dy[i];

                    // Paso 2: ¿La coordenada vecina es válida en el tablero?
                    if (board.isValid(nextX, nextY))
                    {
                        int newGCost = current.gCost + 1; // Cada paso cuesta 1
                        int nextIdx = nextY * Board::SIZE + nextX;

                        // Paso 3: ¿Encontramos un camino más corto a este vecino?
                        if (newGCost < minCosts[nextIdx])
                        {
                            minCosts[nextIdx] = newGCost;
                            
                            // Calculamos nueva heurística y añadimos a la cola
                            int newH = std::abs(nextY - targetRow);
                            openSet.push({nextX, nextY, newGCost, newH});
                        }
                    }
                }
            }
        }

        // 5. Si la cola se vacía y nunca retornamos true, es que no hay camino
        return false;
    }
}
