export default {
  plugins: {
    awesome: {
      options: {
        sections: ["charts"],
        layout: "split",
        charts: [
          {
            type: "trend",
            dataType: "status",
            mode: "percent"
          },
          {
            type: "bar",
            dataType: "statusTrend",
            title: "Status changes between runs",
            mode: "raw"
          },
          {
            type: "treemap",
            dataType: "successRateDistribution"
          },
          {
            type: "heatmap",
            title: "Stability by layer"
          },
          {
            type: "funnel",
            dataType: "testingPyramid"
          }
        ]
      }
    }
  }
};
