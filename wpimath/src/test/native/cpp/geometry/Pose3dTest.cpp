// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include <cmath>

#include <wpi/array.h>

#include "frc/geometry/Pose3d.h"
#include "gtest/gtest.h"

using namespace frc;

TEST(Pose3dTest, TestTransformByRotations) {
  const double kEpsilon = 1E-9;

  const Pose3d initialPose{0_m, 0_m, 0_m, Rotation3d{0_deg, 0_deg, 0_deg}};
  const Transform3d transform1{Translation3d{0_m, 0_m, 0_m},
                               Rotation3d{90_deg, 45_deg, 0_deg}};
  const Transform3d transform2{Translation3d{0_m, 0_m, 0_m},
                               Rotation3d{-90_deg, 0_deg, 0_deg}};
  const Transform3d transform3{Translation3d{0_m, 0_m, 0_m},
                               Rotation3d{0_deg, -45_deg, 0_deg}};

  Pose3d finalPose = initialPose.TransformBy(transform1)
                         .TransformBy(transform2)
                         .TransformBy(transform3);

  EXPECT_NEAR(finalPose.Rotation().X().value(),
              initialPose.Rotation().X().value(), kEpsilon);
  EXPECT_NEAR(finalPose.Rotation().Y().value(),
              initialPose.Rotation().Y().value(), kEpsilon);
  EXPECT_NEAR(finalPose.Rotation().Z().value(),
              initialPose.Rotation().Z().value(), kEpsilon);
}

TEST(Pose3dTest, TransformBy) {
  Eigen::Vector3d zAxis{0.0, 0.0, 1.0};

  const Pose3d initial{1_m, 2_m, 0_m, Rotation3d{zAxis, 45.0_deg}};
  const Transform3d transform{Translation3d{5_m, 0_m, 0_m},
                              Rotation3d{zAxis, 5_deg}};

  const auto transformed = initial + transform;

  EXPECT_DOUBLE_EQ(1.0 + 5.0 / std::sqrt(2.0), transformed.X().value());
  EXPECT_DOUBLE_EQ(2.0 + 5.0 / std::sqrt(2.0), transformed.Y().value());
  EXPECT_DOUBLE_EQ(transformed.Rotation().Z().value(),
                   units::radian_t{50_deg}.value());
}

TEST(Pose3dTest, RelativeTo) {
  Eigen::Vector3d zAxis{0.0, 0.0, 1.0};

  const Pose3d initial{0_m, 0_m, 0_m, Rotation3d{zAxis, 45_deg}};
  const Pose3d final{5_m, 5_m, 0_m, Rotation3d{zAxis, 45_deg}};

  const auto finalRelativeToInitial = final.RelativeTo(initial);

  EXPECT_DOUBLE_EQ(5.0 * std::sqrt(2.0), finalRelativeToInitial.X().value());
  EXPECT_DOUBLE_EQ(0.0, finalRelativeToInitial.Y().value());
  EXPECT_DOUBLE_EQ(0.0, finalRelativeToInitial.Rotation().Z().value());
}

TEST(Pose3dTest, Equality) {
  Eigen::Vector3d zAxis{0.0, 0.0, 1.0};

  const Pose3d a{0_m, 5_m, 0_m, Rotation3d{zAxis, 43_deg}};
  const Pose3d b{0_m, 5_m, 0_m, Rotation3d{zAxis, 43_deg}};
  EXPECT_TRUE(a == b);
}

TEST(Pose3dTest, Inequality) {
  Eigen::Vector3d zAxis{0.0, 0.0, 1.0};

  const Pose3d a{0_m, 5_m, 0_m, Rotation3d{zAxis, 43_deg}};
  const Pose3d b{0_m, 5_ft, 0_m, Rotation3d{zAxis, 43_deg}};
  EXPECT_TRUE(a != b);
}

TEST(Pose3dTest, Minus) {
  Eigen::Vector3d zAxis{0.0, 0.0, 1.0};

  const Pose3d initial{0_m, 0_m, 0_m, Rotation3d{zAxis, 45_deg}};
  const Pose3d final{5_m, 5_m, 0_m, Rotation3d{zAxis, 45_deg}};

  const auto transform = final - initial;

  EXPECT_DOUBLE_EQ(5.0 * std::sqrt(2.0), transform.X().value());
  EXPECT_DOUBLE_EQ(0.0, transform.Y().value());
  EXPECT_DOUBLE_EQ(0.0, transform.Rotation().Z().value());
}

TEST(Pose3dTest, ToPose2d) {
  Pose3d pose{1_m, 2_m, 3_m, Rotation3d{20_deg, 30_deg, 40_deg}};
  Pose2d expected{1_m, 2_m, 40_deg};

  EXPECT_EQ(expected, pose.ToPose2d());
}

TEST(Pose3dTest, ComplexTwists) {
  wpi::array<Pose3d, 5> initial_poses{
      Pose3d{0.698303_m, -0.959096_m, 0.271076_m,
             Rotation3d{Quaternion{0.86403, -0.076866, 0.147234, 0.475254}}},
      Pose3d{0.634892_m, -0.765209_m, 0.117543_m,
             Rotation3d{Quaternion{0.84987, -0.070829, 0.162097, 0.496415}}},
      Pose3d{0.584827_m, -0.590303_m, -0.02557_m,
             Rotation3d{Quaternion{0.832743, -0.041991, 0.202188, 0.513708}}},
      Pose3d{0.505038_m, -0.451479_m, -0.112835_m,
             Rotation3d{Quaternion{0.816515, -0.002673, 0.226182, 0.531166}}},
      Pose3d{0.428178_m, -0.329692_m, -0.189707_m,
             Rotation3d{Quaternion{0.807886, 0.029298, 0.257788, 0.529157}}},
  };

  wpi::array<Pose3d, 5> final_poses{
      Pose3d{-0.230448_m, -0.511957_m, 0.198406_m,
             Rotation3d{Quaternion{0.753984, 0.347016, 0.409105, 0.379106}}},
      Pose3d{-0.088932_m, -0.343253_m, 0.095018_m,
             Rotation3d{Quaternion{0.638738, 0.413016, 0.536281, 0.365833}}},
      Pose3d{-0.107908_m, -0.317552_m, 0.133946_m,
             Rotation3d{Quaternion{0.653444, 0.417069, 0.465505, 0.427046}}},
      Pose3d{-0.123383_m, -0.156411_m, -0.047435_m,
             Rotation3d{Quaternion{0.652983, 0.40644, 0.431566, 0.47135}}},
      Pose3d{-0.084654_m, -0.019305_m, -0.030022_m,
             Rotation3d{Quaternion{0.620243, 0.429104, 0.479384, 0.44873}}},
  };

  for (size_t i = 0; i < initial_poses.size(); i++) {
    auto start = initial_poses[i];
    auto end = final_poses[i];

    auto twist = start.Log(end);
    auto start_exp = start.Exp(twist);

    auto eps = 1E-5;

    EXPECT_NEAR(start_exp.X().value(), end.X().value(), eps);
    EXPECT_NEAR(start_exp.Y().value(), end.Y().value(), eps);
    EXPECT_NEAR(start_exp.Z().value(), end.Z().value(), eps);
    EXPECT_NEAR(start_exp.Rotation().GetQuaternion().W(),
                end.Rotation().GetQuaternion().W(), eps);
    EXPECT_NEAR(start_exp.Rotation().GetQuaternion().X(),
                end.Rotation().GetQuaternion().X(), eps);
    EXPECT_NEAR(start_exp.Rotation().GetQuaternion().Y(),
                end.Rotation().GetQuaternion().Y(), eps);
    EXPECT_NEAR(start_exp.Rotation().GetQuaternion().Z(),
                end.Rotation().GetQuaternion().Z(), eps);
  }
}

TEST(Pose3dTest, TwistNaN) {
  const Pose3d initial{6.32_m, 4.12_m, 0.00_m,
                       Rotation3d{Quaternion{-0.9999999999999999, 0.0, 0.0,
                                             1.9208309264993548E-8}}};
  const Pose3d final{6.33_m, 4.15_m, 0.00_m,
                     Rotation3d{Quaternion{-0.9999999999999999, 0.0, 0.0,
                                           2.416890209039172E-8}}};

  auto twist = initial.Log(final);

  EXPECT_FALSE(std::isnan(twist.dx.value()));
  EXPECT_FALSE(std::isnan(twist.dy.value()));
  EXPECT_FALSE(std::isnan(twist.dz.value()));
  EXPECT_FALSE(std::isnan(twist.rx.value()));
  EXPECT_FALSE(std::isnan(twist.ry.value()));
  EXPECT_FALSE(std::isnan(twist.rz.value()));
}
