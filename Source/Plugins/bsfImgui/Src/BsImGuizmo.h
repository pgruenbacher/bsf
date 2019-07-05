#pragma once

#include "ImGuizmo.h"

/*===================================================
=            Logic specific for ImGuizmo            =
===================================================*/

namespace bs {
	
void manipulateMatrix(Matrix4& matrix, const SPtr<Camera> camera,
                      ImGuizmo::OPERATION operation, ImGuizmo::MODE mode);

void manipulateMatrix(Matrix4& matrix, const Matrix4& proj, const Matrix4& view,
                      ImGuizmo::OPERATION operation, ImGuizmo::MODE mode);

/**
 * Updates transform by generating interface fields as well as 3d gizmo.
 *
 * @param[in]	transform 	modified by the interface
 * @param[in] camera      used for determining the view projection for the
 * 												interface perspective.
 */
void EditTransform(Transform& transform, const SPtr<Camera> camera);

}  // namespace bs::ct