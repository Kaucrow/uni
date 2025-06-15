import { Collider } from "./Collider.js";
import { Trigger } from "./Trigger.js";

export class CollisionSystem {
  static FUNCS = {
    STOP: (source) => {
      source.x = source.oldX;
      source.y = source.oldY;
      source.setAction('idle');
      source.animator.setAnimation(source.animator.currentAnimation.replace("run", "walk"));
      source.animator.frameX = 1;
      source.animator.animationTimer = 0;
    }    
  };

  constructor() {
    this.colliders = [];
  }

  register(collider) {
    if (!(collider instanceof Collider)) throw Error("Collider must be a Collider object")
    this.colliders.push(collider); 
  }

  checkCollisions(source) {
    let collisions = [];
    for (const other of this.colliders) {
      if (other.parent === source || collisions.includes(other.parent)) continue;

      if (this.#checkCollision(source, other)) {
        collisions.push(other.parent)
      }
    }
  }

  // Uses Separating Axis Theorem to check for collisions
  #checkCollision(source, other) {
    if (!source.colliders) throw Error("Attempted to check for collisions but no colliders were found on source object");

    // Check all colliders on the source object against the other object
    for (const collider of source.colliders) {
      // Skip collision check if the colliders do not interact
      let groupNames = [];

      if (collider.interactions.some(item => item.group === other.group)) {
        groupNames.push(other.group);
      }
      if (collider.interactions.some(item => item.group === 'any')) {
        groupNames.push('any');
      }

      if (groupNames.length === 0) continue;

      const sourceEdgeNormals = collider.uniqueEdgeNormals;
      const otherEdgeNormals = other.uniqueEdgeNormals;

      // Get all unique axes to test (from both colliders)
      const edgeNormals = this.#mergeUniqueEdgeNormals(sourceEdgeNormals, otherEdgeNormals);
      const sourceVertices = collider.vertices;
      const otherVertices = other.vertices;

      // Assume collision until a separating axis is found
      let collision = true;

      // Test each axis for separation
      for (const axis of edgeNormals) {
        // Get the x and y of the normalized direction vector axis
        const axisX = axis.end.x;
        const axisY = axis.end.y;

        // Project all vertices of both shapes onto the axis
        let sourceMin = Infinity, sourceMax = -Infinity;
        let otherMin = Infinity, otherMax = -Infinity;

        // Project source vertices
        for (const vertex of sourceVertices) {
          const projection = vertex.x * axisX + vertex.y * axisY; // Dot product
          sourceMin = Math.min(sourceMin, projection);
          sourceMax = Math.max(sourceMax, projection);
        }

        // Project other vertices
        for (const vertex of otherVertices) {
          const projection = vertex.x * axisX + vertex.y * axisY; // Dot product
          otherMin = Math.min(otherMin, projection);
          otherMax = Math.max(otherMax, projection);
        }

        // Check for overlap
        if (sourceMax < otherMin || otherMax < sourceMin) {
          // Found a separating axis - no collision
          collision = false;
          break;
        }
      }

      // If no separating axis was found, the shapes collide
      if (collision) {
        if (!(other.isTrigger)) {
          groupNames.forEach((groupName) => {
            const onCollide = this.#getOnCollide(groupName, collider);
            source.collisionCallbacks.push(onCollide);
          })
        } else if (other.isTrigger) {
          if (source.tags.includes(other.interactsWith)) {
            other.update(source, true);
          }
        }

        return true;
      // If the collider is a trigger, update it
      } else if (other.isTrigger) {
        if (source.tags.includes(other.interactsWith)) {
          other.update(source, false);
        }
      }
    }

    // No collisions found
    return false;
  }

  #getOnCollide(groupName, collider) {
    const interaction = collider.interactions.find(item => item.group === groupName);
    if (interaction?.onCollide) {
      return interaction.onCollide;
    }
  }

  #mergeUniqueEdgeNormals(sourceEdgeNormals, otherEdgeNormals) {
    const axes = []; // Used to deduplicate axes
    const epsilon = 1e-6;

    // Helper: Check if a normal is already in `axes`
    const isUniqueNormal = (normal) => {
      for (const axis of axes) {
        // Cross product to check parallelism (if ≈0, normals are parallel)
        const cross = axis.end.x * normal.end.y - axis.end.y * normal.end.x;
        if (Math.abs(cross) < epsilon) {
          return false; // Not unique (parallel to an existing axis)
        }
      }
      return true; // Unique
    };

    // Process source edges
    for (const normal of sourceEdgeNormals) {
      if (isUniqueNormal(normal)) {
        axes.push(normal);
      }
    }

    // Process collider edges
    for (const normal of otherEdgeNormals) {
      if (isUniqueNormal(normal)) {
        axes.push(normal);
      }
    }

    return axes;
  }
}