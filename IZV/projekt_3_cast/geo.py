#!/usr/bin/python3.8
# coding=utf-8

import pandas as pd
import seaborn as sns
import numpy as np
import geopandas
import sklearn.cluster
import sklearn.mixture
import contextily as ctx
import os
from matplotlib import pyplot as plt

__author__ = "Mirka Kolarikova"


def make_geo(df: pd.DataFrame) -> geopandas.GeoDataFrame:
    """ Konvertovani dataframe do GeoDataFrame se spravnym kodovani"""
    return geopandas.GeoDataFrame(df,
                                  geometry=geopandas.points_from_xy(df["d"],
                                                                    df["e"]),
                                  crs="EPSG:5514")


def plot_geo(gdf: geopandas.GeoDataFrame, fig_location: str = None,
             show_figure: bool = False):
    """ Vykresleni grafu s dvemi podgrafy podle lokality nehody
    v ZLK kraji """

    gdf_ZLK = gdf[gdf["region"] == "ZLK"]
    gdf_ZLK = gdf_ZLK.to_crs("epsg:3857")

    titles = ["Nehody v ZLK kraji: v obci", "Nehody v ZLK kraji: mimo obec"]
    colors = ["red", "deepskyblue"]

    fig, axes = plt.subplots(1, 2, figsize=(15, 7))
    for i, axe in enumerate(axes):
        gdf_ZLK[gdf_ZLK["p5a"] == i + 1].plot(ax=axe,
                                              markersize=1,
                                              color=colors[i])
        axe.axis("off")
        axe.set_title(titles[i])
        ctx.add_basemap(axe, crs=gdf_ZLK.crs.to_string(),
                        source=ctx.providers.Stamen.TonerLite,
                        alpha=0.9)
    plt.tight_layout()

    if fig_location:
        (dirname, filename) = os.path.split(fig_location)
        if dirname:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def plot_cluster(gdf: geopandas.GeoDataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """ Vykresleni grafu s lokalitou vsech nehod v ZLK kraji
    shlukovanych do clusteru """
    gdf_ZLK = gdf[gdf["region"] == "ZLK"]
    gdf_ZLK = gdf_ZLK.set_geometry(gdf_ZLK.centroid).to_crs(epsg=3857)
    gdf_ZLK = gdf_ZLK[~gdf_ZLK.geometry.is_empty]  # smazat prazdne
    coords = np.dstack([gdf_ZLK.geometry.x, gdf_ZLK.geometry.y]).reshape(-1, 2)
    db = sklearn.cluster.MiniBatchKMeans(n_clusters=20).fit(coords)
    gdf4 = gdf_ZLK.copy()
    gdf4["cluster"] = db.labels_
    gdf4 = gdf4.dissolve(by="cluster", aggfunc={"p1": "count"})
    gdf_coords = geopandas.GeoDataFrame(
    	geometry=geopandas.points_from_xy(db.cluster_centers_[:, 0],
                                          db.cluster_centers_[:, 1]))
    gdf5 = gdf4.merge(gdf_coords, left_on="cluster",
                      right_index=True).set_geometry("geometry_y")

    plt.figure(figsize=(12, 8))
    ax = plt.gca()
    gdf_ZLK.plot(ax=ax, markersize=0.8, color="grey")
    gdf5.plot(ax=ax, markersize=gdf5["p1"], column="p1",
              legend=True, alpha=0.6)
    ctx.add_basemap(ax, crs="epsg:3857", source=ctx.providers.Stamen.TonerLite)
    ax.set_title("Nehody v ZLK kraji")
    ax.axis("off")

    if fig_location:
        (dirname, filename) = os.path.split(fig_location)
        if dirname:
            if not os.path.exists(dirname):
                os.makedirs(dirname)
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    gdf = make_geo(pd.read_pickle("accidents.pkl.gz"))
    plot_geo(gdf, "geo1.pdf")
    plot_cluster(gdf, "geo2.pdf")
