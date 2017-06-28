////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014-2016, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
// Written by the LBANN Research Team (B. Van Essen, et al.) listed in
// the CONTRIBUTORS file. <lbann-dev@llnl.gov>
//
// LLNL-CODE-697807.
// All rights reserved.
//
// This file is part of LBANN: Livermore Big Artificial Neural Network
// Toolkit. For details, see http://software.llnl.gov/LBANN or
// https://github.com/LLNL/LBANN.
//
// Licensed under the Apache License, Version 2.0 (the "Licensee"); you
// may not use this file except in compliance with the License.  You may
// obtain a copy of the License at:
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the license.
//
// lbann_layer .h .cpp - Parent class for all layer types
////////////////////////////////////////////////////////////////////////////////

#ifndef LBANN_LAYER_TRANSFORM_HPP_INCLUDED
#define LBANN_LAYER_TRANSFORM_HPP_INCLUDED

#include "lbann/layers/lbann_layer.hpp"
#include <string>
#include <vector>

namespace lbann {

  //template <data_layout T_layout>
class transform : public Layer {

 public:
  transform(int index, 
            // int numPrevNeurons,
            // int numNeurons,
           lbann_comm *comm,
           int mini_batch_size)
    : Layer(index, comm, mini_batch_size) {

  }

  virtual ~transform() {}

  template<data_layout T_layout> inline void initialize_distributed_matrices() {
    Layer::initialize_distributed_matrices<T_layout>();
  }

#if 0
  static std::string weight_initialization_name(weight_initialization id);

  virtual void forwardProp(void);
  virtual void backProp(void);
  virtual bool update(void);
  virtual void summarize(lbann_summary& summarizer, int step);

  /** Validate that the setup is reasonable. */
  virtual void check_setup(void);

  /** Return (a view of) the weights/biases matrix for this layer. */
  virtual ElMat& get_weights_biases(void) {
    return *m_weights;
  }
  /** Return (a view of) the weights/biases gradient matrix for this layer. */
  virtual ElMat& get_weights_biases_gradient(void) {
    return *m_weights_gradient;
  }

  ElMat *fp_output(void);
  ElMat *bp_output(void);
  void setup_fp_input(ElMat *fp_input);
  void setup_bp_input(ElMat *bp_input);

  void set_prev_layer_type(layer_type type);
  void set_next_layer_type(layer_type type);
  bool using_gpus(void) const;
  void set_prev_layer_using_gpus(bool using_gpus);
  void set_next_layer_using_gpus(bool using_gpus);
#ifdef __LIB_CUDNN
  std::vector<DataType *> *fp_output_d(void);
  std::vector<DataType *> *bp_output_d(void);
  void setup_fp_input_d(std::vector<DataType *> *fp_input_d);
  void setup_bp_input_d(std::vector<DataType *> *bp_input_d);
#endif

#endif
#if 0
 protected:

  /** Setup views of the matrices for the layer's forward propagation. */
  virtual void fp_set_std_matrix_view(void);
#if 0
  /** Setup views of the matrices for the layer's backward propagation. */
  virtual void bp_set_std_matrix_view(void);
#endif
  /** Apply the layer's linear update in forward propagation. */
  virtual void fp_linearity(void) {}
  /** Handle the layer's linearity in backward propagation. */
  virtual void bp_linearity(void) {}
  /** Apply the layer's nonlinearity in forward propagation. */
  virtual void fp_nonlinearity(void);
  /** Handle the layer's nonlinearity in backward propagation. */
  virtual void bp_nonlinearity(void);
#endif
};
}


#endif // LBANN_LAYER_TRANSFORM_HPP_INCLUDED
