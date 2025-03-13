#!/bin/bash
#SBATCH --job-name=haileyC
#SBATCH --partition=Centaurus
#SBATCH --time=01:00:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --mem=16G
#SBATCH --output=result.txt

./bfs_crawler "Tom Hanks" 2 | tail -n 1
./bfs_crawler "The Green Mile" 2 | tail -n 1
./bfs_crawler "Prohibition" 1 | tail -n 1
./bfs_crawler "Paul Sanchez" 2 | tail -n 1
./bfs_crawler "Cast Away" 2 | tail -n 1
