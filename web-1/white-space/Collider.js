import { Vector2 } from "./Vector.js";

export class Collider {
  constructor(config, parent) {
    if (!parent) throw Error("Missing reference to parent GameObject");

    this.parent = parent;

    if (config.edges) {
      this._edges = config.edges;

      if (!this.#isClosedPolygon(config.edges)) {
        throw Error("Collider's edges don't form a closed polygon");
      }
      if (this.#isConcavePolygon(config.edges)) {
        throw Error("Collider's edges cannot form a concave polygon");
      }
    } else {
      throw Error("Missing collider edges");
    }

    if (config.group) {
      this.group = config.group;
    } else {
      throw Error("Missing collider group");
    }

    this.interactions = (config.interactions || []).map(interaction => ({
      group: interaction.group,
      onCollide: interaction.onCollide || (() => {}),
    }));

    // Draw properties
    this.drawProps = null;
    if (config.draw) {
      this.drawProps = new ColliderDrawProps(config.draw);
    }
  }

  // Get edges in world-space (parent position applied)
  get edges() {
    return this._edges.map(edge => {
      // Convert offset points to arrays for Vector2 constructor
      const start = [
        edge.start.x + this.parent.x,
        edge.start.y + this.parent.y
      ];
      const end = [
        edge.end.x + this.parent.x,
        edge.end.y + this.parent.y
      ];
      
      if (!Array.isArray(start) || !Array.isArray(end)) {
        console.error("Invalid edge:", edge); // Log problematic edge
        throw new Error("Edge points must be convertible to arrays");
      }

      // Return a new Vector2 instance
      return new Vector2(start, end);
    });
  }

  // Get a specific edge in world-space
  getWorldEdge(index) {
    const edge = this._edges[index];
    return {
      start: {
        x: edge.start.x + this.parent.x,
        y: edge.start.y + this.parent.y,
      },
      end: {
        x: edge.end.x + this.parent.x,
        y: edge.end.y + this.parent.y,
      },
    };
  }

  get uniqueEdgeNormals() {
    const axes = []; // Used to deduplicate axes
    const epsilon = 1e-6;

    for (const edge of this.edges) {
      let isDuplicate = false;

      const edgeDirV = edge.directionVector;
      const normal = new Vector2(-edgeDirV.end.y, edgeDirV.end.x).normalized; // Perpendicular normal

      for (const axis of axes) {
        let cross = axis.end.x * normal.end.y - axis.end.y * normal.end.x;
        if (Math.abs(cross) < epsilon) {
          isDuplicate = true; // Not unique (parallel to an existing axis)
          break;
        }
      }

      if (!isDuplicate) {
        axes.push(normal);
      }
    }

    return axes;
  }

  draw(ctx) {
    if (!this.drawProps) return;

    const vertices = this.vertices;

    ctx.beginPath();
    ctx.moveTo(vertices[0].x, vertices[1].y);

    for (let i = 1; i < vertices.length; i++) {
      ctx.lineTo(vertices[i].x, vertices[i].y);
    }

    ctx.closePath();

    ctx.fillStyle = this.drawProps.fillStyle;
    ctx.fill();

    ctx.strokeStyle = this.drawProps.strokeStyle;
    ctx.lineWidth = this.drawProps.lineWidth;
    ctx.stroke();
  }

  get vertices() {
    const edges = this.edges;
    const vertices = [];
    for (let i = 0; i < edges.length; i++) {
      vertices.push(edges[i].start);
    }
    return vertices;
  }

  // Checks if the collider's edges form a closed polygon
  #isClosedPolygon(edges) {
    if (edges.length < 3) return false; // Need at least 3 edges to form a polygon

    // Check if edges form a continuous chain
    for (let i = 0; i < edges.length; i++) {
      const currentEnd = edges[i].end;
      const nextStart = edges[(i + 1) % edges.length].start;
      
      if (currentEnd.x !== nextStart.x || currentEnd.y !== nextStart.y) {
        return false;
      }
    }
    return true;
  }

  #isConcavePolygon(edges) {
    if (edges.length < 3) return false;
    
    // Convert edges to vertices array
    const vertices = this.vertices;
    
    let sign = 0;
    const n = vertices.length;
    
    for (let i = 0; i < n; i++) {
      const p1 = vertices[i];
      const p2 = vertices[(i + 1) % n];
      const p3 = vertices[(i + 2) % n];

      // Calculate cross product
      const cross = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);

      if (cross !== 0) {
        if (sign === 0) {
          // First non-zero cross product sets the expected sign
          sign = cross > 0 ? 1 : -1;
        } else if ((cross > 0 && sign === -1) || (cross < 0 && sign === 1)) {
          // Sign change detected - concave polygon
          return true;
        }
      }
    }

    // No sign changes detected - convex polygon
    return false;
  }
}

export class ColliderDrawProps {
  constructor(config) {
    this.strokeStyle = config.strokeStyle || 'red';
    this.fillStyle = 'transparent';
    if (config.fill) {
      this.fillStyle = this.#reduceOpacity(this.strokeStyle);
    }
    this.lineWidth = config.lineWidth || 1;
  }

  #reduceOpacity(color, alpha = 0.4) {
    const tempDiv = document.createElement('div');
    tempDiv.style.color = color;
    document.body.appendChild(tempDiv);
    const rgb = getComputedStyle(tempDiv).color;
    document.body.removeChild(tempDiv);

    if (rgb.startsWith('rgba')) {
        return rgb.replace(/[\d.]+\)$/, alpha + ')');
    }
    return rgb.replace('rgb', 'rgba').replace(')', `, ${alpha})`);
  }
}