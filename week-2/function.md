### Function: `dijkstra()`

**Purpose**: Implements Dijkstra's algorithm to find the shortest path from a source node to all other nodes in the graph.

**Parameters**:
- `graph`: The adjacency matrix representing the graph.
- `source`: The starting node for the algorithm.

**Return Value**: An array of shortest distances from the source to each node.

**Detailed Description**:
The `dijkstra()` function initializes the distance array and sets the distance to the source node to zero. It uses a min-heap to efficiently retrieve the next node with the smallest tentative distance, updating the distances to its neighbors accordingly until all nodes have been processed.
