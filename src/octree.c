#include <stdlib.h>
#include <stdbool.h>
#include "octree.h"

bool octree_node_add(octree_node_t* node, octree_data_t* obj);
bool octree_node_divide(octree_node_t* node);

// Allocate a octree result object
// Return allocated object, and NULL on failure
octree_results_t* octree_results_init() {
    octree_results_t* results = (octree_results_t*) malloc(sizeof(octree_results_t));

    if ( results == NULL ) {
        return NULL;
    }

    results->data = (octree_data_t**) malloc(OCTREE_RESULT_SIZE * sizeof(octree_data_t*));

    if ( results->data == NULL ) {
        free(results);
        return NULL;
    }

    results->index = 0;
    results->size = OCTREE_RESULT_SIZE;

    return results;
}

// Grow the result object by the static size
// Return true on success
bool octree_results_grow(octree_results_t* result) {
    result->size += OCTREE_RESULT_SIZE;

    // Grow the size
    octree_data_t** tmp = realloc(result->data, result->size * sizeof(octree_data_t*));

    if ( tmp == NULL ) {
        // result->data will be cleaned up with rest of data
        return false;
    }

    result->data = tmp;
    return true;
}

// Add an object into the results object
// Return true on success
bool octree_results_add(octree_results_t* result, octree_data_t* data) {
    if ( result->index >= result->size ) {
        if ( !octree_results_grow(result) ) {
            return false;
        }
    }

    result->data[result->index++] = data;
    return true;
}

// Release the memory from an octree results object
void octree_results_free(octree_results_t* result) {
    if ( result->data ) {
        free(result->data);
    }

    free(result);
}

// Allocate and instantiate an octree node
// Return allocated node, and NULL on failure
octree_node_t* octree_node_init(octree_obj_t obj) {
    octree_node_t* node = (octree_node_t*) malloc(sizeof(octree_node_t));

    if ( node == NULL ) {
        return NULL;
    }

    node->obj = obj;

    return node;
}

// Determine whether or not an object intersects with a node using AABB
// Return true on intersection
inline bool octree_obj_intersects(octree_obj_t* obj1, octree_obj_t* obj2) {
           // Check X
    return obj1->x < obj2->x + obj2->width &&
           obj1->x + obj1->width > obj2->x &&
           // Check Y
           obj1->y < obj2->y + obj2->height &&
           obj1->y + obj1->height > obj2->y &&
           // Check Z
           obj1->z < obj2->z + obj2->depth &&
           obj1->z + obj1->depth > obj2->z;
}

// Add an object into a node
// Return true on success
bool octree_node_add(octree_node_t* node, octree_data_t* obj) {
    if ( node->leafs[0] == NULL ) {
        // Attempt to insert into children
        for ( int i = 0; i < OCTREE_MAX_CHILDREN; i++ ) {
            if ( node->children[i] == NULL ) {
                node->children[i] = obj;
                return true;
            }
        }

        // Failed to insert into children, divide.
        if ( !octree_node_divide(node) ) {
            return false;
        }
    }

    // Add into all intersecting leafs.
    for ( int i = 0; i < 8; i++ ) {
        if ( octree_obj_intersects(&node->leafs[i]->obj, &obj->obj) ) {
            octree_node_add(node->leafs[i], obj);
        }
    }

    return true;
}

// Subdivide the octree and move it's children into it's legs
// Return true on success
bool octree_node_divide(octree_node_t* node) {
    // Already branched
    if ( node->leafs[0] != NULL ) {
        return false;
    }

    float x = node->obj.x;
    float y = node->obj.y;
    float z = node->obj.z;

    // New dimmensions
    float new_w = node->obj.width / 2.0;
    float new_h = node->obj.height / 2.0;
    float new_d = node->obj.depth / 2.0;

    // New offsets
    float mid_x = x + new_w;
    float mid_y = y + new_h;
    float mid_z = z + new_d;

    // Lower quad
    node->leafs[0] = octree_node_init((octree_obj_t) {
        x, y, z,
        new_w, new_h, new_d
    });

    node->leafs[1] = octree_node_init((octree_obj_t) {
        mid_x, y, z,
        new_w, new_h, new_d
    });

    node->leafs[2] = octree_node_init((octree_obj_t) {
        x, mid_y, z,
        new_w, new_h, new_d
    });

    node->leafs[3] = octree_node_init((octree_obj_t) {
        mid_x, mid_y, z,
        new_w, new_h, new_d
    });

    // Upper quad
    node->leafs[4] = octree_node_init((octree_obj_t) {
        x, y, mid_z,
        new_w, new_h, new_d
    });

    node->leafs[5] = octree_node_init((octree_obj_t) {
        mid_x, y, mid_z,
        new_w, new_h, new_d
    });

    node->leafs[6] = octree_node_init((octree_obj_t) {
        x, mid_y, mid_z,
        new_w, new_h, new_d
    });

    node->leafs[7] = octree_node_init((octree_obj_t) {
        mid_x, mid_y, mid_z,
        new_w, new_h, new_d
    });

    // Make sure all leaves were allocated correctly
    for ( int i = 0; i < 8; i++ ) {
        if ( node->leafs[i] == NULL ) {
            return false;
        }
    }

    // Move the children into the leaves
    for ( int i = 0; i < OCTREE_MAX_CHILDREN; i++ ) {
        if ( node->children[i] == NULL ) {
            continue;
        }

        // Insert the child into the leafs
        if ( !octree_node_add(node, node->children[i]) ) {
            return false;
        }

        // Clear old pointer
        node->children[i] = NULL;
    }

    return true;
}

// Query a node for objects within a node
// Return true on success (even if no objects add)
bool octree_node_query(octree_node_t* node, octree_obj_t* query, octree_results_t* result) {
    // Make sure we intersect
    if ( !octree_obj_intersects(&node->obj, query) ) {
        return true;
    }

    if ( node->children[0] != NULL ) {
        // Add the children into the results object
        for ( int i = 0; i < OCTREE_MAX_CHILDREN; i++ ) {
            if ( node->children[i] == NULL ) {
                continue;
            }

            // Make sure the child intersects with the query
            if ( !octree_obj_intersects(&node->children[i]->obj, query) ) {
                continue;
            }

            // Add the child in
            if ( !octree_results_add(result, node->children[i]) ) {
                return false;
            }
        }
    }
    else if ( node->leafs[0] != NULL ) {
        // Look into the leafs
        for ( int i = 0; i < 8; i++ ) {
            if ( !octree_node_query(node->leafs[i], query, result) ) {
                return false;
            }
        }
    }

    return true;
}

// Release a node's memory, recursively freeing it's children and leafs
void octree_node_free(octree_node_t* node) {
    if ( node->children[0] != NULL ) {
        // Release children
        for ( int i = 0; i < OCTREE_MAX_CHILDREN; i++ ) {
            free(node->children[i]);
        }
    }

    if ( node->leafs[0] != NULL ) {
        // Release leafs
        for ( int i = 0; i < 8; i++ ) {
            octree_node_free(node->leafs[i]);
        }
    }

    free(node);
}

// Allocate and setup an octree
// Return allocated obj, and NULL on failure
octree_t* octree_init(float width, float height, float depth) {
    octree_t* octree = (octree_t*) malloc(sizeof(octree_t));

    if ( octree == NULL ) {
        return NULL;
    }

    octree->root = octree_node_init((octree_obj_t) {
        0, 0, 0,
        width, height, depth
    });

    return octree;
}

// Insert an object into the octree
// Return true on success
bool octree_add(octree_t* octree, void* ptr, octree_obj_t obj) {
    // Allocate a data object for the object
    octree_data_t* data = (octree_data_t*) malloc(sizeof(octree_data_t));

    if ( data == NULL ) {
        return false;
    }

    data->obj = obj;
    data->data = ptr;

    return octree_node_add(octree->root, data);
}

// Search an octree for a specific boundry
// Return results object on success, NULL on failure.
octree_results_t* octree_search(octree_t* octree, octree_obj_t query) {
    octree_results_t* results = octree_results_init();

    if ( results == NULL ) {
        return NULL;
    }

    // Query the tree
    if ( !octree_node_query(octree->root, &query, results) ) {
        // Free results on failure
        octree_results_free(results);
        return NULL;
    }

    return results;
}

// Release all memory allocated by an octree
void octree_free(octree_t* octree) {
    if ( octree->root != NULL ) {
        octree_node_free(octree->root);
    }

    free(octree);
}
