"""Hyperparameter Optimization Class"""

import sklearn
from models.Models import Model
from models.sklearnModels import SklearnModel
from models.kerasModels import KerasModel
from metrics.Metrics import Metric
from Datasets.Datasets import Dataset
from typing import Dict, Any, Optional, Tuple
from functools import reduce
from operator import mul
import itertools
import collections
import numpy as np
import random
import shutil
import tempfile
import os
from sklearn.model_selection import GridSearchCV, RandomizedSearchCV, StratifiedKFold
from keras.wrappers.scikit_learn import KerasClassifier, KerasRegressor



def _convert_hyperparam_dict_to_filename(hyper_params: Dict[str, Any]) -> str:
    """Function that converts a dictionary of hyperparameters to a string that can be a filename.
    Parameters
    ----------
    hyper_params: Dict
      Maps string of hyperparameter name to int/float/string/list etc.
    Returns
    -------
    filename: str
      A filename of form "_key1_value1_value2_..._key2..."
    """
    filename = ""
    keys = sorted(hyper_params.keys())
    for key in keys:
        filename += "_%s" % str(key)
        value = hyper_params[key]
        if isinstance(value, int):
            filename += "_%s" % str(value)
        elif isinstance(value, float):
            filename += "_%f" % value
        else:
            filename += "%s" % str(value)
    return filename


def validate_metrics(metrics):
    '''Validate single and multi metrics'''
    if isinstance(metrics, dict):
        all_metrics = []
        for m in metrics.values():
            if m in sklearn.metrics.SCORERS.keys() or isinstance(m, sklearn.metrics._scorer._PredictScorer):
                all_metrics.append(m)
            else:
                print(m, ' is not a valid scoring function. Use sorted(sklearn.metrics.SCORERS.keys()) '
                         'to get valid options.')
    else:
        if metrics in sklearn.metrics.SCORERS.keys() or isinstance(metrics, sklearn.metrics._scorer._PredictScorer):
            all_metrics = metrics
        else:
            print('WARNING: ', metrics, ' is not a valid scoring function. '
                                       'Use sorted(sklearn.metrics.SCORERS.keys()) to get valid options.')

    if not metrics:
        metrics = 'accuracy'
        print('Using accuracy instead and ', metrics, ' on validation!\n \n')

    return metrics


class HyperparamOpt(object):
    """Abstract superclass for hyperparameter search classes.
    """

    def __init__(self, model_builder: Model, mode: str = None):
        """Initialize Hyperparameter Optimizer.
        Note this is an abstract constructor which should only be used by subclasses.

        Parameters
        ----------
        model_builder: constructor function.
            This parameter must be constructor function which returns an
            object which is an instance of `Models`. This function
            must accept two arguments, `model_params` of type `dict` and
            'model_dir', a string specifying a path to a model directory.
        """
        if self.__class__.__name__ == "HyperparamOpt":
            raise ValueError("HyperparamOpt is an abstract superclass and cannot be directly instantiated. "
                             "You probably want to instantiate a concrete subclass instead.")
        self.model_builder = model_builder
        self.mode = mode


    def hyperparam_search(self,
                          params_dict: Dict[str, Any],
                          train_dataset: Dataset,
                          valid_dataset: Dataset,
                          metric: Metric,
                          use_max: bool = True,
                          logdir: Optional[str] = None,
                          **kwargs) -> Tuple[Model, Dict[str, Any], Dict[str, float]]:
      """Conduct Hyperparameter search.
      This method defines the common API shared by all hyperparameter
      optimization subclasses. Different classes will implement
      different search methods but they must all follow this common API.

      Parameters
      ----------
      params_dict: Dict
        Dictionary mapping strings to values. Note that the
        precise semantics of `params_dict` will change depending on the
        optimizer that you're using.
      train_dataset: Dataset
        dataset used for training
      valid_dataset: Dataset
        dataset used for validation(optimization on valid scores)
      metric: Metric
        metric used for evaluation
      use_max: bool, optional
        If True, return the model with the highest score.
      logdir: str, optional
        The directory in which to store created models. If not set, will
        use a temporary directory.
      Returns
      -------
      Tuple[`best_model`, `best_hyperparams`, `all_scores`]
      """
      raise NotImplementedError


class HyperparamOpt_Valid(HyperparamOpt):
    """
    Provides simple grid hyperparameter search capabilities.
    This class performs a grid hyperparameter search over the specified
    hyperparameter space.
    """

    def hyperparam_search(self,
                          params_dict: Dict,
                          train_dataset: Dataset,
                          valid_dataset: Dataset,
                          metric: Metric,
                          n_iter_search: int = 15,
                          n_jobs: int = 1,
                          verbose: int = 0,
                          use_max: bool = True,
                          logdir: Optional[str] = None,
                          **kwargs):

        """Perform hyperparams search according to params_dict.
        Each key to hyperparams_dict is a model_param. The values should
        be a list of potential values for that hyperparam.
        Parameters
        ----------
        params_dict: Dict
            Maps hyperparameter names (strings) to lists of possible
            parameter values.
        train_dataset: Dataset
            dataset used for training
        valid_dataset: Dataset
            dataset used for validation(optimization on valid scores)
        metric: Metric
            metric used for evaluation
        n_iter_search: int or None, optional
            Number of random combinations of parameters to test, if None performs complete grid search
        use_max: bool, optional
            If True, return the model with the highest score.
        logdir: str, optional
            The directory in which to store created models. If not set, will
            use a temporary directory.
        Returns
        -------
        Tuple[`best_model`, `best_hyperparams`, `all_scores`]
        """

        if self.mode is None:
            #TODO: better way of doint this
            if len(set(train_dataset.y)) > 2:
                model = KerasRegressor(build_fn=self.model_builder, **kwargs)
                self.mode = 'regression'
            else:
                model = KerasClassifier(build_fn=self.model_builder, **kwargs)
                self.mode = 'classification'
        elif self.mode == 'classification':
            model = KerasClassifier(build_fn=self.model_builder, **kwargs)
        elif self.mode == 'regression':
            model = KerasRegressor(build_fn=self.model_builder, **kwargs)
        else : raise ValueError('Model operation mode can only be classification or regression!')

        print('MODE: ', self.mode)
        hyperparams = params_dict.keys()
        hyperparam_vals = params_dict.values()
        for hyperparam_list in params_dict.values():
            assert isinstance(hyperparam_list, collections.Iterable)

        number_combinations = reduce(mul, [len(vals) for vals in hyperparam_vals])

        if use_max:
            best_validation_score = -np.inf
        else:
            best_validation_score = np.inf

        # To make sure that the number of iterations is lower or equal to the number of max hypaparameter combinations
        len_params = sum(1 for x in itertools.product(*params_dict.values()))
        if n_iter_search is None or len_params < n_iter_search:
            n_iter_search = len_params
        random_inds = random.sample(range(0, len_params), k=n_iter_search)
        best_hyperparams = None
        best_model, best_model_dir = None, None
        all_scores = {}
        j = 0
        print("Fitting %d random models from a space of %d possible models." % (
        len(random_inds), number_combinations))
        for ind, hyperparameter_tuple in enumerate(itertools.product(*hyperparam_vals)):
            if ind in random_inds:
                j += 1
                model_params = {}
                print("Fitting model %d/%d" % (j, len(random_inds)))
                hyper_params = dict(zip(hyperparams, hyperparameter_tuple))
                for hyperparam, hyperparam_val in zip(hyperparams, hyperparameter_tuple):
                    model_params[hyperparam] = hyperparam_val
                print("hyperparameters: %s" % str(model_params))

                if logdir is not None:
                    model_dir = os.path.join(logdir, str(ind))
                    print("model_dir is %s" % model_dir)
                    try:
                        os.makedirs(model_dir)
                    except OSError:
                        if not os.path.isdir(model_dir):
                            print("Error creating model_dir, using tempfile directory")
                            model_dir = tempfile.mkdtemp()
                else:
                    model_dir = tempfile.mkdtemp()

                try :
                    model = SklearnModel(self.model_builder(**model_params), model_dir)

                except Exception as e:
                    model = KerasModel(self.model_builder(**model_params), model_dir)

                model.fit(train_dataset)

                try:
                    model.save()
                except Exception as e:
                    print(e)

                multitask_scores = model.evaluate(valid_dataset, [metric])
                valid_score = multitask_scores[metric.name]
                hp_str = _convert_hyperparam_dict_to_filename(hyper_params)
                all_scores[hp_str] = valid_score

                if (use_max and valid_score >= best_validation_score) or (
                        not use_max and valid_score <= best_validation_score):
                    best_validation_score = valid_score
                    best_hyperparams = hyperparameter_tuple
                    if best_model_dir is not None:
                        shutil.rmtree(best_model_dir)
                    best_model_dir = model_dir
                    best_model = model
                else:
                    shutil.rmtree(model_dir)

                print("Model %d/%d, Metric %s, Validation set %s: %f" % (
                    j, len(random_inds), metric.name, j, valid_score))
                print("\tbest_validation_score so far: %f" % best_validation_score)

        if best_model is None:
            print("No models trained correctly.")
            # arbitrarily return last model
            best_model, best_hyperparams = model, hyperparameter_tuple
            return best_model, best_hyperparams, all_scores

        multitask_scores = best_model.evaluate(train_dataset, [metric])
        train_score = multitask_scores[metric.name]
        print("Best hyperparameters: %s" % str(best_hyperparams))
        print("train_score: %f" % train_score)
        print("validation_score: %f" % best_validation_score)
        return best_model, best_hyperparams, all_scores



class HyperparamOpt_CV(HyperparamOpt):
    """
    Provides simple grid hyperparameter search capabilities.
    This class performs a grid hyperparameter search over the specified
    hyperparameter space.
    """

    def hyperparam_search(self,
                          model_type: str,
                          params_dict: Dict,
                          train_dataset: Dataset,
                          metric: Metric,
                          cv: int = 3,
                          n_iter_search: int = 15,
                          n_jobs: int = 1,
                          verbose: int = 0,
                          logdir: Optional[str] = None,
                          **kwargs):

        """Perform hyperparams search according to params_dict.
        Each key to hyperparams_dict is a model_param. The values should
        be a list of potential values for that hyperparam.
        Parameters
        ----------
        model_type: str
            string identifying the type of model (sklearn or keras)
        params_dict: Dict
            Maps hyperparameter names (strings) to lists of possible
            parameter values.
        train_dataset: Dataset
            dataset used for training
        metric: Metric
            metric used for evaluation
        cv: int
            number of folds to perform in the cross validation
        n_iter_search: int or None, optional
            Number of random combinations of parameters to test, if None performs complete grid search
        logdir: str, optional
            The directory in which to store created models. If not set, will
            use a temporary directory.
        Returns
        -------
        Tuple['best_model', 'best_hyperparams', 'all_scores']
        """
        # TODO: better way of doing this
        if self.mode is None:
            if len(set(train_dataset.y)) > 2:
                self.mode = 'regression'
            else:
                self.mode = 'classification'
        print('MODEL TYPE: ', model_type)
        #diferentiate sklearn model from keras model
        if model_type == 'keras':
            if self.mode == 'classification':
                model = KerasClassifier(build_fn=self.model_builder, **kwargs)

            elif self.mode == 'regression':
                model = KerasRegressor(build_fn=self.model_builder, **kwargs)

            else : raise ValueError('Model operation mode can only be classification or regression!')

        elif model_type == 'sklearn':
            model = self.model_builder()
            #model = SklearnModel(self.model_builder, self.mode)

        else : raise ValueError('Only keras and sklearn models are accepted.')

        metrics = validate_metrics(metric)

        number_combinations = reduce(mul, [len(vals) for vals in params_dict.values()])
        if number_combinations > n_iter_search:
            print("Fitting %d random models from a space of %d possible models." % (n_iter_search, number_combinations))
            if self.mode == 'classification':
                grid = RandomizedSearchCV(estimator = model, param_distributions = params_dict,
                                          scoring = metrics, n_jobs=n_jobs, cv=StratifiedKFold(n_splits=cv),
                                          verbose=verbose, n_iter = n_iter_search, refit=True)

            else: grid = RandomizedSearchCV(estimator = model, param_distributions = params_dict,
                                            scoring = metrics, n_jobs=n_jobs, cv=cv,
                                            verbose=verbose, n_iter = n_iter_search, refit=True)
        else :
            if self.mode == 'classification':
                grid = GridSearchCV(estimator = model, param_grid = params_dict,
                                    scoring = metrics, n_jobs=n_jobs, cv=StratifiedKFold(n_splits=cv), verbose=verbose,
                                    refit=True)

            else: grid = RandomizedSearchCV(estimator = model, param_distributions = params_dict,
                                            scoring = metrics, n_jobs=n_jobs, cv=cv,
                                            verbose=verbose, n_iter = n_iter_search, refit=True)

        #print(train_dataset.X.shape, train_dataset.X.shape[0]/cv)
        grid_result = grid.fit(train_dataset.X, train_dataset.y)
        print(grid_result.best_estimator_)
        print("\n \n Best %s: %f using %s" % (metrics, grid_result.best_score_, grid_result.best_params_))
        means = grid_result.cv_results_['mean_test_score']
        stds = grid_result.cv_results_['std_test_score']
        params = grid_result.cv_results_['params']
        for mean, stdev, param in zip(means, stds, params):
            print("\n %s: %f (%f) with: %r \n" % (metrics, mean, stdev, param))

        if model_type == 'keras':
            best_model = KerasModel(self.model_builder, self.mode, **grid_result.best_params_)
            print('Fitting best model!')
            best_model.fit(train_dataset)
            return best_model, grid_result.best_params_, grid_result.cv_results_
        elif model_type == 'sklearn':
            print(grid_result.best_estimator_)
            return SklearnModel(grid_result.best_estimator_, mode=self.mode), grid_result.best_params_, grid_result.cv_results_

