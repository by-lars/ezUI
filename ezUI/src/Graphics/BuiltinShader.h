#pragma once

namespace ez {
	constexpr char DEFAULT_VERTEX_SHADER[] = R"(
		#version 450 core

		struct InstanceData {
			vec4 A;
			vec4 B;
		};

		layout(std430, binding = 0) buffer TransformSSBO
		{
			InstanceData sData[];
		};
	
		uniform mat4 uProj;
		uniform mat4 uView;
		uniform float uOffset;

		const float Quad[24] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		out vec3 vColor;

		void main() {
			int baseVertex = (gl_VertexID % 6) * 4;
			vec3 pos = vec3(Quad[baseVertex], Quad[baseVertex+1], 0.0f);
			vec2 tex = vec2(Quad[baseVertex+2], Quad[baseVertex+3]);

			vColor = vec3(tex, 1.0f);

			InstanceData data = sData[gl_VertexID / 6];
			mat4 model;
			model[0] = vec4(data.A.w, 0.0, 0.0, 0.0);
			model[1] = vec4(0.0, data.B.x, 0.0, 0.0);
			model[2] = vec4(0.0, 0.0, 1.0, 0.0);
			model[3] = vec4(data.A.x, data.A.y, data.A.z + uOffset, 1.0);


			//model[3][0] = data.Translation.x;
			//model[3][1] = data.Translation.y;
			//model[3][2] = data.Translation.z + uOffset;

			gl_Position = uProj * uView * model * vec4(pos, 1.0f);
		}
	)";


	constexpr char DEFAULT_FRAGMENT_SHADER[] = R"(
		#version 450 core

		in vec3 vColor;
		out vec4 FragColor;

		void main() {
			FragColor = vec4(vColor, 1.0f);
		}
	)";
}