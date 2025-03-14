import os
import shutil
import tempfile
from typing import List, Optional, Sequence
import numpy as np
from Datasets.Datasets import Dataset
from evaluator.Evaluator import Evaluator
from metrics.Metrics import Metric

from sklearn.base import BaseEstimator


class Model(BaseEstimator):
    """Abstract base class for ML/DL models.
    """

    def __init__(self, model=None, model_dir: Optional[str]= None,  **kwargs) -> None:
        """Abstract class for all models.
        This is an abstact class and should not be invoked directly.

        Parameters
        ----------
        model: object
            Wrapper around ScikitLearn/Keras/Tensorflow model object.
        model_dir: str, optional (default None)
            Path to directory where model will be stored. If not specified,
            model will be stored in a temporary directory.
        """

        if self.__class__.__name__ == "Model":
            raise ValueError("This constructor is for an abstract class and should never be called directly. Can only call from subclass constructors.")

        self.model_dir_is_temp = False

        if model_dir is not None:
            if not os.path.exists(model_dir):
                os.makedirs(model_dir)
        else:
            model_dir = tempfile.mkdtemp()
            self.model_dir_is_temp = True

        self.model_dir = model_dir
        self.model = model
        self.model_class = model.__class__

    def __del__(self):
        if 'model_dir_is_temp' in dir(self) and self.model_dir_is_temp:
            shutil.rmtree(self.model_dir)

    def fit_on_batch(self,  X: Sequence, y: Sequence):
        """Perform a single step of training.
        Parameters
        ----------
        X: np.ndarray
            the inputs for the batch
        y: np.ndarray
            the labels for the batch
        """
        raise NotImplementedError("Each class model must implement its own fit_on_batch method.")

    def predict_on_batch(self, X: Sequence):
        """
        Makes predictions on given batch of new data.
        Parameters
        ----------
        X: np.ndarray
            array of features
        """
        raise NotImplementedError("Each class model must implement its own predict_on_batch method.")

    def reload(self) -> None:
        """
        Reload trained model from disk.
        """
        raise NotImplementedError("Each class model must implement its own reload method.")


    def get_model_filename(self, model_dir: str) -> str:
        """
        Given model directory, obtain filename for the model itself.
        """
        return os.path.join(model_dir, "model.joblib")


    def get_params_filename(self, model_dir: str) -> str:
        """
        Given model directory, obtain filename for the model itself.
        """
        return os.path.join(model_dir, "model_params.joblib")

    def save(self) -> None:
        """Function for saving models.
        Each subclass is responsible for overriding this method.
        """
        raise NotImplementedError("Each class model must implement its own save method.")

    def fit(self, dataset: Dataset):
        """
        Fits a model on data in a Dataset object.
        Parameters
        ----------
        dataset: Dataset
            the Dataset to train on
        """
        raise NotImplementedError("Each class model must implement its own fit method.")

    def predict(self, dataset: Dataset) -> np.ndarray:
        """
        Uses self to make predictions on provided Dataset object.
        Parameters
        ----------
        dataset: Dataset
            Dataset to make prediction on

        Returns
        -------
        np.ndarray
            A numpy array of predictions.
        """
        y_preds = []
        for (X_batch, _, _, ids_batch) in dataset.iterbatches(deterministic=True):
            n_samples = len(X_batch)
            y_pred_batch = self.predict_on_batch(X_batch)
            # Discard any padded predictions
            y_pred_batch = y_pred_batch[:n_samples]
            y_preds.append(y_pred_batch)
        y_pred = np.concatenate(y_preds)
        return y_pred

    def evaluate(self,
                dataset: Dataset,
                metrics: List[Metric],
                per_task_metrics: bool = False,
                n_classes: int = 2):
        """
        Evaluates the performance of this model on specified dataset.

        Parameters
        ----------
        dataset: Dataset
            Dataset object.
        metrics: Metric / List[Metric]
            The set of metrics provided.
        per_task_metrics: bool, optional (default False)
            If true, return computed metric for each task on multitask dataset.
        n_classes: int, optional (default None)
            If specified, will use `n_classes` as the number of unique classes.

        Returns
        -------
        multitask_scores: dict
            Dictionary mapping names of metrics to metric scores.
        all_task_scores: dict, optional
            If `per_task_metrics == True` is passed as a keyword argument,
            then returns a second dictionary of scores for each task
            separately.
        """

        evaluator = Evaluator(self, dataset)

        return evaluator.compute_model_performance(metrics,
                                                   per_task_metrics=per_task_metrics,
                                                   n_classes=n_classes)

    def get_task_type(self) -> str:
        """
        Currently models can only be classifiers or regressors.
        """
        raise NotImplementedError()

    def get_num_tasks(self) -> int:
        """
        Get number of tasks.
        """
        raise NotImplementedError()


