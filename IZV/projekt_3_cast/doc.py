#!/usr/bin/python3.8
# coding=utf-8

from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np
import os

__author__ = "Mirka Kolarikova"


def plot_kind(df: pd.DataFrame, fig_location: str = None,
              show_figure: bool = False):
    """ Vykresleni grafu o skode nehody v zavislosti
    na typu nehody"""

    # style graphs
    sns.set_style("darkgrid")
    fig, axes = plt.subplots(nrows=3, ncols=1, figsize=(5, 8.27))

    df_kind = df[['p1', 'p6', 'p53', 'region']].copy()
    df_kind['p6'] = df_kind['p6'].replace(
        {'0': 'jiné',
         '1': 's jedoucím vozidlem',
         '2': 'se zaparkovaným vozidlem',
         '3': 's pevnou překážkou',
         '4': 's chodcem',
         '5': 's lesní zvěří',
         '6': 's domácím zvířetem',
         '7': 's vlakem',
         '8': 's tramvají',
         '9': 'havárie'})
    df_kind['p53'] = df_kind['p53'].astype(int).mul(100)

    kind_PHA = df_kind[df_kind["region"] == "PHA"]
    kind_JHM = df_kind[df_kind["region"] == "JHM"]

    data_ALL = (df_kind.groupby(['p6']).agg({'p53': 'mean'}).reset_index())
    data_PHA = (kind_PHA.groupby(['p6']).agg({'p53': 'mean'}).reset_index())
    data_JHM = (kind_JHM.groupby(['p6']).agg({'p53': 'mean'}).reset_index())

    data = [data_PHA, data_JHM, data_ALL]
    title = ['Průměrná škoda nehod v PHA kraji v Kč',
             'Průměrná škoda nehod v JHM kraji v Kč',
             'Průměrná škoda nehod v celé ČR v Kč']

    # plot 4 graphs
    for i in range(3):
        g = sns.barplot(ax=axes[i],
                        data=data[i],
                        x='p53',
                        y='p6',
                        saturation=1,
                        orient='h')
        axes[i].set_title(title[i], fontweight="bold")
        axes[i].set(ylabel='', xlabel='')
        axes[i].spines['bottom'].set_color('#404040')
        axes[i].spines['left'].set_color('#404040')
    plt.tight_layout()

    # save or show
    if fig_location:
        (dirname, filename) = os.path.split(fig_location)
        # create folder if doesnt exist
        if dirname:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def make_table(df: pd.DataFrame):
    """ Vypsani 3 tabulek na stdout o poctu nehod a prumerne skode"""
    df_table = df[['p1', 'p6', 'p53', 'region']].copy()

    df_table['p6'] = df_table['p6'].replace(
        {'0': 'jiné',
         '1': 'srážka s jedoucím nekolejovým vozidlem',
         '2': 'srážka se zaparkovaným vozidlem',
         '3': 'srážka s pevnou překážkou',
         '4': 'srážka s chodcem',
         '5': 'srážka s lesní zvěří',
         '6': 'srážka s domácím zvířetem',
         '7': 'srážka s vlakem',
         '8': 'srážka s tramvají',
         '9': 'havárie'})
    df_table['p53'] = df_table['p53'].astype(int).mul(100)

    kind_PHA = df_table[df_table["region"] == "PHA"]
    kind_JHM = df_table[df_table["region"] == "JHM"]

    data_ALL = (df_table.groupby('p6')
                        .agg({'p53': ['count', 'mean']})
                        .reset_index())
    data_PHA = (kind_PHA.groupby('p6')
                        .agg({'p53': ['count', 'mean']})
                        .reset_index())
    data_JHM = (kind_JHM.groupby('p6')
                        .agg({'p53': ['count', 'mean']})
                        .reset_index())

    data = [data_ALL, data_PHA, data_JHM]
    title = ["Tabulka celá ČR", "Tabulka PHA", "Tabulka JHM"]

    for i in range(3):
        print(title[i])
        print("typ nehody\tcount\tmean")
        for row in data[i].values:
            for item in row:
                print(item, end="\t")
            print()
        print()


def print_used_data():
    print("Srážka s vlakem průměr zaokrouhleno: 40 000 Kč")
    print("Škoda PHA vs JHM vlak: PHA 3.3x více")
    print("Srážka s chodcem vs lesní zvěří: lesní zvěř 4x více")
    print("Počet srážek lesní zvěře JHM vs PHA: PHA 5x více")


if __name__ == "__main__":
    df = pd.read_pickle("accidents.pkl.gz")
    plot_kind(df, fig_location="fig.pdf", show_figure=True)
    make_table(df)
    print_used_data()
