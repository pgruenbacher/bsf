shader ImguiBasic
{
	blend
	{
		target	
		{
			enabled = true;
			color = { srcA, srcIA, add };
			writemask = RGB;
		};
	};	
	
	depth
	{
		read = false;
		write = false;
	};
	
	code
	{
		cbuffer ImguiParams
		{
			float4x4 gWorldTransform;
			// float gInvViewportWidth;
			// float gInvViewportHeight;
			// float gViewportYFlip;
		}	

		void vsmain(
			in float2 inPos : POSITION,
			in float2 uv : TEXCOORD0,
			in float4 inColor : COLOR0,

			out float4 oPosition : SV_Position,
			out float2 oUv : TEXCOORD0,
			out float4 oColor : COLOR0)
		{
			float4 tfrmdPos = mul(gWorldTransform, float4(inPos.xy, 0, 1));
			oPosition = tfrmdPos;
			oUv = uv;
			oColor = inColor;
		}

		[alias(gMainTexture)]
		SamplerState gMainTexSamp;
		Texture2D gMainTexture;

		float4 fsmain(in float4 inPos : SV_Position, float2 uv : TEXCOORD0, float4 inColor : COLOR0) : SV_Target
		{
			float4 color = gMainTexture.Sample(gMainTexSamp, uv) * inColor;
			return color;
		}
	};
};