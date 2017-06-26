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
////////////////////////////////////////////////////////////////////////////////

#include "lbann/metrics/lbann_metric_top_k_categorical_accuracy.hpp"

namespace lbann {

namespace metrics {

top_k_categorical_accuracy::top_k_categorical_accuracy(
  data_layout data_dist, int top_k, lbann_comm *comm) :
  metric(data_dist, comm), m_top_k(top_k),
  m_gathered_predictions(comm->get_model_grid()),
  m_gathered_ground_truth(comm->get_model_grid()) {
  this->type = metric_type::top_k_categorical_accuracy;

  // Setup the data distribution
  switch (data_dist) {
  case data_layout::MODEL_PARALLEL:
    initialize_model_parallel_distribution();
    break;
  case data_layout::DATA_PARALLEL:
    initialize_data_parallel_distribution();
    break;
  default:
    throw lbann_exception(std::string{} + __FILE__ + " " +
                          std::to_string(__LINE__) +
                          "Invalid data layout selected");
  }
}

top_k_categorical_accuracy::~top_k_categorical_accuracy() {

}

void top_k_categorical_accuracy::initialize_model_parallel_distribution() {
  
}

void top_k_categorical_accuracy::initialize_data_parallel_distribution() {
  
}

void top_k_categorical_accuracy::setup(int num_neurons, int mini_batch_size) {
  metric::setup(num_neurons, mini_batch_size);
}

void top_k_categorical_accuracy::fp_set_std_matrix_view(
  int64_t cur_mini_batch_size) {

}

double top_k_categorical_accuracy::compute_metric(
  ElMat& predictions_v, ElMat& ground_truth_v) {
  // Simple approach: Gather all predictions to the model master.
  // TODO: Better to gather only the top k predictions from each model.
  int num_errors = 0;
  if (this->m_comm->am_model_master()) {
    m_gathered_predictions = predictions_v;
    m_gathered_ground_truth = ground_truth_v;
    const El::Int mbsize = m_gathered_predictions.Width();
    const El::Int num_classes = m_gathered_predictions.Height();
    // Compute the top k predictions in each entry of the minibatch.
    Mat top_predictions(mbsize, m_top_k);
    std::vector<El::Int> indices(num_classes);
    std::iota(indices.begin(), indices.end(), 0);
    for (El::Int mb_idx = 0; mb_idx < mbsize; ++mb_idx) {
      // We want the indices of the top k predictions for this entry.
      // This partially sorts an auxiliary array of indices based on the
      // predictions.
      std::partial_sort(indices.begin(), indices.begin() + m_top_k,
                        indices.end(),
                        [mb_idx, this] (El::Int a, El::Int b) -> bool {
                          return m_gathered_predictions.GetLocal(a, mb_idx) >
                            m_gathered_predictions.GetLocal(b, mb_idx); });
      // Compute the index of the ground truth value.
      El::Int ground_truth_idx = -1;
      for (El::Int class_idx = 0; class_idx < num_classes; ++class_idx) {
        if (m_gathered_ground_truth.GetLocal(class_idx, mb_idx) ==
            DataType(1)) {
          ground_truth_idx = class_idx;
        }
      }
      // Now check if those top indices contain the true value.
      bool found = false;
      for (auto pred = indices.begin(); pred != indices.begin() + m_top_k;
           ++pred) {
        if (*pred == ground_truth_idx) {
          found = true;
          break;
        }
      }
      if (!found) {
        ++num_errors;
      }
    }
    // Broadcast the number of errors to the rest of the models.
    this->m_comm->model_broadcast(0, num_errors);
  } else {
    num_errors = this->m_comm->model_broadcast<int>(0);
  }
  return num_errors;
}

double top_k_categorical_accuracy::report_metric(execution_mode mode) {
  statistics *stats = get_statistics(mode);
  double errors_per_epoch = stats->m_error_per_epoch;
  long samples_per_epoch = stats->m_samples_per_epoch;

  double accuracy = (double)(samples_per_epoch - errors_per_epoch) / samples_per_epoch * 100;
  string score = std::to_string(accuracy);

  return accuracy;
}

double top_k_categorical_accuracy::report_lifetime_metric(
  execution_mode mode) {
  statistics *stats = get_statistics(mode);
  double total_error = stats->m_total_error;
  long total_num_samples = stats->m_total_num_samples;

  double accuracy = (double)(total_num_samples - total_error) / total_num_samples * 100;
  string score = std::to_string(accuracy);

  return accuracy;
}

}  // namespace metrics

}  // namespace lbann
