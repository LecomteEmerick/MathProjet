#include "Matrix.h"

Matrix::Matrix(float matrix[2][2])
{
	std::vector<std::vector<float>> newMatrice;
	std::vector<float> newColonne;
	for (std::vector<float>::size_type ligne = 0; ligne < 2; ligne++)
	{
		for (std::vector<float>::size_type col = 0; col < 2; col++)
		{
			newColonne.push_back(matrix[ligne][col]);
		}
		newMatrice.push_back(newColonne);
	}
	this->matrix_ = newMatrice;
}

Matrix::Matrix(float matrix[3][3])
{
	std::vector<std::vector<float>> newMatrice;
	std::vector<float> newColonne;
	for (std::vector<float>::size_type ligne = 0; ligne < 3; ligne++)
	{
		for (std::vector<float>::size_type col = 0; col < 3; col++)
		{
			newColonne.push_back(matrix[ligne][col]);
		}
		newMatrice.push_back(newColonne);
		newColonne.clear();
	}
	this->matrix_ = newMatrice;
}

Matrix::Matrix(std::vector<std::vector<float>> matrix)
{
	this->matrix_ = matrix;
}

float Matrix::determinant()
{
	float det = 0;

	if (this->matrix_.size() == 2 && this->matrix_[0].size() == 2)
	{
		return this->matrix_[0][0] * this->matrix_[1][1] - this->matrix_[0][1] * this->matrix_[1][0];
	}
	else
	{
		//on construit la sous matrice de en �liminant la colonne 0
		//colonne 
		for (std::vector<float>::size_type col = 0; col < this->matrix_[0].size(); col++)
		{
			std::vector<float> colonne;
			std::vector<std::vector<float>> sousMatrice;
			for (int i = 1; i < this->matrix_.size(); i++)
			{
				colonne = std::vector<float>();
				//ligne
				for (std::vector<float>::size_type j = 0; j < this->matrix_[i].size(); j++)
				{
					if (j != col)
						colonne.push_back(this->matrix_[i][j]);
				}
				sousMatrice.push_back(colonne);
			}
			det += pow((-1), col) *(this->matrix_[0][col] * Matrix(sousMatrice).determinant());
		}
		return det;
	}
}

//Ne fonctionne que pour les matrices 2,2 ou 3,3
Matrix Matrix::comatrice()
{
	std::vector<std::vector<float>> newMatrice;
	std::vector<float> newColonne;
	for (std::vector<float>::size_type ligne = 0; ligne < this->matrix_.size(); ligne++)
	{
		for (std::vector<float>::size_type col = 0; col < this->matrix_[ligne].size(); col++)
		{
			std::vector<float> colonne;
			std::vector<std::vector<float>> sousMatrice;
			for (int i = 0; i < this->matrix_.size(); i++)
			{
				colonne = std::vector<float>();
				//ligne
				if (i != ligne)
				{
					for (std::vector<float>::size_type j = 0; j < this->matrix_[i].size(); j++)
					{
						if (j != col)
							colonne.push_back(this->matrix_[i][j]);
					}
					sousMatrice.push_back(colonne);
				}
			}
			if (sousMatrice.size() == 2 && sousMatrice[0].size() == 2)
			{
				newColonne.push_back(pow((-1), ligne + col) * (sousMatrice[0][0] * sousMatrice[1][1] - sousMatrice[0][1] * sousMatrice[1][0]));
			}
			else if (sousMatrice.size() == 1 && sousMatrice[0].size() == 1)
			{
				newColonne.push_back(pow((-1), ligne + col) * sousMatrice[0][0]);
			}
		}
		newMatrice.push_back(newColonne);
		newColonne.clear();
	}
	return Matrix(newMatrice);
}

Matrix Matrix::transpose()
{
	std::vector<std::vector<float>> tMatrix;
	std::vector<float> tColumn;
	for (std::vector<float>::size_type ligne = 0; ligne < this->matrix_.size(); ligne++)
	{
		for (std::vector<float>::size_type col = 0; col < this->matrix_[ligne].size(); col++)
		{
			tColumn.push_back(this->matrix_[col][ligne]);
		}
		tMatrix.push_back(tColumn);
		tColumn.clear();
	}
	return Matrix(tMatrix);
}

void Matrix::printMatrix()
{
	std::cout << "matrice : " << std::endl;
	for (std::vector<float>::size_type ligne = 0; ligne < this->matrix_.size(); ligne++)
	{
		for (std::vector<float>::size_type col = 0; col < this->matrix_[ligne].size(); col++)
		{
			std::cout << this->matrix_[col][ligne] << " ";
		}
		std::cout << std::endl;
	}
}

Matrix::~Matrix()
{
}