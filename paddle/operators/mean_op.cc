/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserve.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include "paddle/operators/mean_op.h"

namespace paddle {
namespace operators {

class MeanOp : public framework::OperatorWithKernel {
 public:
  using framework::OperatorWithKernel::OperatorWithKernel;

 protected:
  void InferShape(const framework::InferShapeContext &ctx) const override {
    PADDLE_ENFORCE_NOT_NULL(ctx.InputVar("X"),
                            "Input of MeanOp must be initialized.");
    ctx.Output<Tensor>("Out")->Resize({1});
  }
};

class MeanOpMaker : public framework::OpProtoAndCheckerMaker {
 public:
  MeanOpMaker(framework::OpProto *proto, framework::OpAttrChecker *op_checker)
      : OpProtoAndCheckerMaker(proto, op_checker) {
    AddInput("X", "The input of mean op");
    AddOutput("Out", "The output of mean op").NotInGradient();
    AddComment("Mean Operator");
  }
};

class MeanGradOp : public framework::OperatorWithKernel {
 public:
  using framework::OperatorWithKernel::OperatorWithKernel;

 protected:
  void InferShape(const framework::InferShapeContext &ctx) const override {
    ctx.Output<Tensor>(framework::GradVarName("X"))
        ->Resize(ctx.Input<Tensor>("X")->dims());
  }
};

}  // namespace operators
}  // namespace paddle

namespace ops = paddle::operators;
REGISTER_OP(mean, ops::MeanOp, ops::MeanOpMaker, mean_grad, ops::MeanGradOp);
REGISTER_OP_CPU_KERNEL(mean,
                       ops::MeanKernel<paddle::platform::CPUPlace, float>);
REGISTER_OP_CPU_KERNEL(mean_grad,
                       ops::MeanGradKernel<paddle::platform::CPUPlace, float>);
