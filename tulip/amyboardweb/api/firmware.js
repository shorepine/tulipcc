const ALLOWED_HOSTS = new Set([
  "github.com",
  "objects.githubusercontent.com",
  "raw.githubusercontent.com",
]);

export default async function handler(req, res) {
  if (req.method === "OPTIONS") {
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Access-Control-Allow-Methods", "GET, OPTIONS");
    res.setHeader("Access-Control-Allow-Headers", "Content-Type");
    res.status(204).end();
    return;
  }

  const urlParam = req.query?.url;
  if (!urlParam) {
    res.status(400).send("Missing url parameter");
    return;
  }

  let targetUrl;
  try {
    targetUrl = new URL(urlParam);
  } catch (err) {
    res.status(400).send("Invalid url");
    return;
  }

  if (!ALLOWED_HOSTS.has(targetUrl.hostname)) {
    res.status(403).send("URL not allowed");
    return;
  }

  try {
    const upstream = await fetch(targetUrl.toString(), {
      redirect: "follow",
      headers: {
        "User-Agent": "amyboard-firmware-proxy",
      },
    });

    if (!upstream.ok) {
      res.status(upstream.status).send("Upstream fetch failed");
      return;
    }

    const arrayBuffer = await upstream.arrayBuffer();
    const buffer = Buffer.from(arrayBuffer);
    res.setHeader("Access-Control-Allow-Origin", "*");
    res.setHeader("Content-Type", "application/octet-stream");
    res.setHeader("Content-Length", buffer.length.toString());
    res.status(200).send(buffer);
  } catch (err) {
    res.status(502).send("Proxy fetch failed");
  }
}
