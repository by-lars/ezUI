#pragma once
#include <string>

namespace ez::gfx {
	const std::string GL_QUAD_VERTEX_SHADER = R"(
		#version 450 core

		struct InstanceData {
			vec4 A;
			vec4 B;
			ivec4 C;
		};

		layout(std430, binding = 0) buffer TransformSSBO
		{
			InstanceData sData[];
		};
	
		uniform mat4	uProj;
		uniform mat4	uView;
		uniform float	uOffset;
		out		vec3	vTexCoords;

		const float Quad[24] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		void main() {
			InstanceData data = sData[gl_VertexID / 6];
			int baseVertex = (gl_VertexID % 6) * 4;

			vec3 pos = vec3(Quad[baseVertex], Quad[baseVertex+1], 0.0f);
			vTexCoords = vec3(Quad[baseVertex+2], Quad[baseVertex+3], data.C.x);

			mat4 model;
			model[0] = vec4(data.A.w, 0.0, 0.0, 0.0);
			model[1] = vec4(0.0, data.B.x, 0.0, 0.0);
			model[2] = vec4(0.0, 0.0, 1.0, 0.0);
			model[3] = vec4(data.A.x, data.A.y, data.A.z + uOffset, 1.0);

			gl_Position = uProj * uView * model * vec4(pos, 1.0f);
		}
	)";


	const std::string GL_QUAD_FRAGMENT_SHADER = R"(
		#version 450 core

		in		vec3			vTexCoords;
		out		vec4			FragColor;
		uniform	sampler2DArray	uBrushTex;

		// This approximates the error function, needed for the gaussian integral
		vec4 erf(vec4 x) {
		  vec4 s = sign(x), a = abs(x);
		  x = 1.0 + (0.278393 + (0.230389 + 0.078108 * (a * a)) * a) * a;
		  x *= x;
		  return s - s / (x * x);
		}
		
		// Return the mask for the shadow of a box from lower to upper
		float boxShadow(vec2 lower, vec2 upper, vec2 point, float sigma) {
		  vec4 query = vec4(point - lower, upper - point);
		  vec4 integral = 0.5 + 0.5 * erf(query * (sqrt(0.5) / sigma));
		  return (integral.z - integral.x) * (integral.w - integral.y);
		}

		void main() {
			FragColor = texture(uBrushTex, vTexCoords);
			FragColor *= boxShadow(vec2(0.2, 0.2), vec2(0.8, 0.8), vTexCoords.xy, 0.75);
		}
	)";
}