#pragma once

namespace bs::ct {
void manipulateMatrix(Matrix4& matrix, const Camera& camera,
                      ImGuizmo::OPERATION operation, ImGuizmo::MODE mode);
void manipulateMatrix(Matrix4& matrix, const Matrix4& proj, const Matrix4& view,
                      ImGuizmo::OPERATION operation, ImGuizmo::MODE mode);
void EditTransform(Transform& transform, const Camera& camera);
}  // namespace bs::ct