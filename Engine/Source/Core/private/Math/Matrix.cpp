#include <Math/Matrix.h>
#include <Math/Constants.h>
#include <Math/MathHelper.h>

namespace GameEngine::Core
{
	namespace Math
	{
		Matrix4x4f ViewMatrixLH(
			Vector3f pos,
			Vector3f target,
			Vector3f up
		)
		{
			Vector3f eyeDirection = target - pos;

			assert(eyeDirection != Vector3f::Zero());
			assert(up != Vector3f::Zero());

			eyeDirection = eyeDirection.Normalized();
			
			Vector3f side = up.CrossProduct(eyeDirection);
			side = side.Normalized();

			up = eyeDirection.CrossProduct(side);

			pos = -pos;

			float d0 = side * pos;
			float d1 = up * pos;
			float d2 = eyeDirection * pos;

			Matrix4x4f result;
			
			result.SetElement(side.x, 0, 0);
			result.SetElement(side.y, 1, 0);
			result.SetElement(side.z, 2, 0);
			result.SetElement(d0, 3, 0);

			result.SetElement(up.x, 0, 1);
			result.SetElement(up.y, 1, 1);
			result.SetElement(up.z, 2, 1);
			result.SetElement(d1, 3, 1);

			result.SetElement(eyeDirection.x, 0, 2);
			result.SetElement(eyeDirection.y, 1, 2);
			result.SetElement(eyeDirection.z, 2, 2);
			result.SetElement(d2, 3, 2);

			result.SetElement(0, 0, 3);
			result.SetElement(0, 1, 3);
			result.SetElement(0, 2, 3);
			result.SetElement(1, 3, 3);

			return result;
		}

		Matrix4x4f ProjectionMatrixLH(
			float FovAngleY,
			float AspectRatio,
			float NearZ,
			float FarZ
		)
		{
			assert(NearZ > 0.f && FarZ > 0.f);

			float    SinFov;
			float    CosFov;
			CalculateSinCos(SinFov, CosFov, 0.5f * FovAngleY);

			float Height = CosFov / SinFov;
			float Width = Height / AspectRatio;
			float fRange = FarZ / (FarZ - NearZ);

			Matrix4x4f result;

			result.SetElement(Width, 0, 0);
			result.SetElement(0.0f, 0, 1);
			result.SetElement(0.0f, 0, 2);
			result.SetElement(0.0f, 0, 3);

			result.SetElement(0.0f, 1, 0);
			result.SetElement(Height, 1, 1);
			result.SetElement(0.0f, 1, 2);
			result.SetElement(0.0f, 1, 3);

			result.SetElement(0.0f, 2, 0);
			result.SetElement(0.0f, 2, 1);
			result.SetElement(fRange, 2, 2);
			result.SetElement(1.0f, 2, 3);

			result.SetElement(0.0f, 3, 0);
			result.SetElement(0.0f, 3, 1);
			result.SetElement(-fRange * NearZ, 3, 2);
			result.SetElement(0.0f, 3, 3);

			return result;
		}

		Matrix4x4f RotationMatrixLH(Vector3f angles)
		{
			Matrix4x4f result;

			float cosX = cosf(angles.x);
			float sinX = sinf(angles.x);
			float cosY = cosf(angles.y);
			float sinY = sinf(angles.y);
			float cosZ = cosf(angles.z);
			float sinZ = sinf(angles.z);

			result.SetElement(cosY * cosZ, 0, 0);
			result.SetElement(cosY * sinZ, 0, 1);
			result.SetElement(-sinY, 0, 2);
			result.SetElement(0, 0, 3);

			result.SetElement(sinX * sinY * cosZ - cosX * sinZ, 1, 0);
			result.SetElement(sinX * sinY * sinZ + cosX * cosZ, 1, 1);
			result.SetElement(sinX * cosY, 1, 2);
			result.SetElement(0, 1, 3);

			result.SetElement(cosX * sinY * cosZ + sinX * sinZ, 2, 0);
			result.SetElement(cosX * sinY * sinZ - sinX * cosZ, 2, 1);
			result.SetElement(cosX * cosY, 2, 2);
			result.SetElement(0, 2, 3);

			result.SetElement(1, 3, 3);

			return result;
		}

		Matrix4x4f WorldMatrixLH(Vector3f angles, Vector3f scalingVec, Vector3f translationVec)
		{
			Matrix4x4f rotation = RotationMatrixLH(angles);

			Matrix4x4f scalingMatrix;
			scalingMatrix.SetElement(scalingVec.x, 0, 0);
			scalingMatrix.SetElement(scalingVec.y, 1, 1);
			scalingMatrix.SetElement(scalingVec.z, 2, 2);
			scalingMatrix.SetElement(1, 3, 3);

			Matrix4x4f translationMatrix = Matrix4x4f::Identity();
			translationMatrix.SetElement(translationVec.x, 0, 3);
			translationMatrix.SetElement(translationVec.y, 1, 3);
			translationMatrix.SetElement(translationVec.z, 2, 3);

			return scalingMatrix * rotation * translationMatrix;
		}
	}
}